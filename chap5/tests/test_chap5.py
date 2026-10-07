"""Exercise the actual POSIX executables in an isolated temporary directory."""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class Chapter5Tests(unittest.TestCase):
    def setUp(self):
        previous_umask = os.umask(0o022)
        self.addCleanup(os.umask, previous_umask)
        self.temp = tempfile.TemporaryDirectory(prefix="chap5-")
        self.addCleanup(self.temp.cleanup)
        self.cwd = Path(self.temp.name)
        self.file = self.cwd / "input.txt"
        self.file.write_bytes(b"First line\n\nThird line\nLast line")

    def run_program(self, folder, *args, data="", success=True):
        result = subprocess.run([str(ROOT / folder / "main"), *map(str, args)],
                                input=data, text=True, capture_output=True,
                                cwd=self.cwd, timeout=5)
        if success:
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        else:
            self.assertNotEqual(result.returncode, 0, result.stdout + result.stderr)
        return result

    def test_open_and_missing(self):
        self.assertIn("디스크립터", self.run_program("prob1", self.file).stdout)
        self.run_program("prob1", self.cwd / "missing", success=False)

    def test_large_and_empty_size(self):
        self.file.write_bytes(b"abcdefgh" * 300 + b"!")
        self.assertIn("2401", self.run_program("prob2", self.file).stdout)
        self.file.write_bytes(b"")
        self.assertIn("0 바이트", self.run_program("prob2", self.file).stdout)
        self.run_program("prob2", self.cwd / "missing", success=False)

    def test_copy_binary_truncate_and_alias(self):
        content = bytes(range(256)) * 9 + b"end"
        self.file.write_bytes(content)
        target = self.cwd / "copy"
        self.run_program("prob3", self.file, target)
        self.assertEqual(target.read_bytes(), content)
        self.assertEqual(target.stat().st_mode & 0o777, 0o600)
        self.file.write_bytes(b"short")
        self.run_program("prob3", self.file, target)
        self.assertEqual(target.read_bytes(), b"short")
        self.run_program("prob3", self.file, self.file, success=False)
        self.assertEqual(self.file.read_bytes(), b"short")
        alias = self.cwd / "alias"
        os.link(self.file, alias)
        self.run_program("prob3", self.file, alias, success=False)
        self.assertEqual(self.file.read_bytes(), b"short")
        self.run_program("prob3", self.cwd, target, success=False)
        self.assertEqual(target.read_bytes(), b"short")

    def test_duplicate_offset(self):
        self.run_program("prob4")
        self.assertEqual((self.cwd / "myfile").read_bytes(), b"Hello! Linux\nBye! Linux\n")

    def test_record_store_lookup_update(self):
        record = self.cwd / "student.txt"
        self.run_program("prob5", record, data="1401001 Kim 90\n1401003 Lee 70\n")
        self.assertEqual(record.stat().st_mode & 0o777, 0o640)
        result = self.run_program("prob6", record, data="1401001\nY\n1401002\ny\n0\nY\nxyz\nN\n")
        self.assertIn("Kim", result.stdout)
        self.assertIn("90", result.stdout)
        self.assertIn("레코드 [1401002] 없음", result.stdout)
        self.assertIn("레코드 [0] 없음", result.stdout)
        self.assertIn("입력 오류", result.stdout)
        self.run_program("prob7", record, data="1401001\n99\nY\n1401003\n82\nN\n")
        self.assertIn("99", self.run_program("prob6", record, data="1401001\nN\n").stdout)
        self.assertIn("82", self.run_program("prob6", record, data="1401003\nN\n").stdout)
        before = record.read_bytes()
        self.run_program("prob7", record, data="1401002\nN\n")
        self.run_program("prob7", record, data="1401001\nwrong\nN\n")
        self.assertEqual(record.read_bytes(), before)
        self.run_program("prob5", record, data="1401003 Lee 85\n")
        self.assertIn("99", self.run_program("prob6", record, data="1401001\nN\n").stdout)
        self.run_program("prob5", self.cwd / "bad", data="-1 Kim 90\n", success=False)
        self.run_program("prob5", self.cwd / "bad2", data="1401001 Kim\n", success=False)

    def test_truncated_record(self):
        self.file.write_bytes(b"broken")
        self.run_program("prob6", self.file, data="1401001\n", success=False)
        self.run_program("prob7", self.file, data="1401001\n", success=False)

    def test_oversized_tokens_and_incomplete_update(self):
        record = self.cwd / "student.txt"
        self.run_program("prob5", record, data="1401001 Kim 90\n")
        before = record.read_bytes()
        bad_name = "A" * 23 + "99"
        self.run_program("prob5", record, data=f"1401001 {bad_name} 70\n", success=False)
        self.assertEqual(record.read_bytes(), before)
        bad_score = "0" * 63 + "x"
        self.run_program("prob7", record, data=f"1401001\n{bad_score}\nN\n")
        self.assertEqual(record.read_bytes(), before)
        self.run_program("prob7", record, data="1401001\n", success=False)
        self.assertEqual(record.read_bytes(), before)
        self.assertIn("입력 오류", self.run_program("prob6", record, data="0" * 63 + "x\nN\n").stdout)

    def test_line_selection(self):
        result = self.run_program("exercise1", self.file, data="3\n1,3\n2-4\n*\n")
        self.assertIn("Third line", result.stdout)
        self.assertIn("First line", result.stdout)
        self.assertIn("Last line", result.stdout)
        self.assertEqual(result.stdout.count("Third line"), 4)
        self.assertEqual(result.stdout.count("First line"), 2)
        self.assertEqual(result.stdout.count("Last line"), 2)

    def test_reverse_blank_long_and_empty(self):
        result = self.run_program("exercise2", self.file)
        self.assertEqual(result.stdout, "Last line\nThird line\n\nFirst line")
        self.file.write_text("x" * 4096 + "\ntail\n")
        self.assertEqual(self.run_program("exercise2", self.file).stdout, "tail\n" + "x" * 4096 + "\n")
        self.file.write_bytes(b"")
        self.assertEqual(self.run_program("exercise2", self.file).stdout, "")

    def test_usage(self):
        for folder in ("prob1", "prob2", "prob3", "prob5", "prob6", "prob7", "exercise1", "exercise2"):
            self.run_program(folder, success=False)


if __name__ == "__main__":
    unittest.main(verbosity=2)
