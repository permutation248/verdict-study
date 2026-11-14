from verdict.logging.logger import create_run_dir

def test_create_run_dir_collision_suffix(tmp_path):
    d1 = create_run_dir("x/foo.c", log_dir=tmp_path)
    d2 = create_run_dir("x/foo.c", log_dir=tmp_path)  # same second
    assert d1 != d2
    assert d1.exists() and d2.exists()
    assert d2.name.startswith(d1.name)  # base name + "-2" suffix


def test_trim_long_strings_keeps_structure():
    from verdict.tools.factory import _trim_long_strings

    obj = {"a": "x" * 100, "b": ["y" * 100, 5], "c": {"d": "ok"}}
    out = _trim_long_strings(obj, 10)
    assert out["a"].startswith("xxxxxxxxxx... [truncated")
    assert out["b"][0].startswith("yyyyyyyyyy... [truncated")
    assert out["b"][1] == 5
    assert out["c"]["d"] == "ok"
