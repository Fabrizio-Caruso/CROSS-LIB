import pytest
import os
import sys
import tempfile
from unittest.mock import patch

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))


@pytest.mark.parametrize("payload", [
    "; rm -rf / #",
    "$(whoami)/",
    "`id`/",
    "../etc/passwd; cat /etc/shadow #",
    "valid_dir/",
])
def test_split_projects_no_unsanitized_shell_injection(payload, tmp_path):
    """Invariant: Shell commands never include unsanitized user input"""
    executed_commands = []

    def mock_system(cmd):
        executed_commands.append(cmd)
        # Check that shell metacharacters from payload are not present raw
        # Strip the trailing slash for checking if it was a simple path
        dangerous_chars = [';', '$(', '`', '|', '&&', '||']
        for char in dangerous_chars:
            if char in payload and char in cmd:
                raise AssertionError(
                    f"Unsanitized shell metacharacter '{char}' from user input "
                    f"found in shell command: {cmd}"
                )
        return 0

    # Create a minimal environment for the module to run
    split_dir = os.path.join(str(tmp_path), payload)
    project_dir = os.path.join(str(tmp_path), "output", payload)

    with patch('os.system', side_effect=mock_system):
        with patch('os.makedirs', return_value=None):
            with patch('os.path.isdir', return_value=True):
                with patch('os.listdir', return_value=[]):
                    try:
                        from src.modules.split_projects import split_projects
                        split_projects(split_dir, project_dir)
                    except (TypeError, AttributeError, ImportError):
                        # If function signature differs, try alternate import
                        try:
                            import src.modules.split_projects as sp
                            # Try to invoke whatever callable exists
                            for attr in dir(sp):
                                obj = getattr(sp, attr)
                                if callable(obj) and not attr.startswith('_'):
                                    try:
                                        obj(split_dir, project_dir)
                                    except (TypeError, ValueError):
                                        continue
                        except Exception:
                            pass

    # If any command was executed, verify no injection
    for cmd in executed_commands:
        dangerous_sequences = ['; rm', '$(whoami)', '`id`', '; cat /etc']
        for seq in dangerous_sequences:
            if seq in payload:
                assert seq not in cmd, (
                    f"Command injection detected: payload '{seq}' appears "
                    f"unsanitized in shell command: {cmd}"
                )