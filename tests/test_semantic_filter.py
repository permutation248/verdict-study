"""Tests for ACSL annotation semantic filter."""

import pytest
from verdict.framac.annotation_semantic_filter import (
    check_annotation,
    check_annotations,
    param_names_from_params,
)


@pytest.fixture
def params():
    return {"ctx", "crl", "off", "len", "eaten", "critical"}


class TestCheckAnnotation:
    def test_bare_address_of_pointer_param_in_separated(self, params):
        errors = check_annotation(
            r"\separated(&ctx, crl + (0 .. off + len - 1))", params
        )
        assert len(errors) == 1
        assert "'&ctx'" in errors[0]
        assert "stack slot" in errors[0]

    def test_address_of_field_is_ok(self, params):
        errors = check_annotation(
            r"\separated(&ctx->sig_alg, crl + (0 .. len - 1))", params
        )
        assert errors == []

    def test_bare_address_of_non_pointer_param_in_valid(self, params):
        errors = check_annotation(r"\valid(&len)", params)
        assert len(errors) == 1
        assert "'&len'" in errors[0]

    def test_address_of_non_param_is_ok(self, params):
        errors = check_annotation(
            r"\separated(&local_var, crl + (0 .. len - 1))", params
        )
        assert errors == []

    def test_no_memory_predicate_is_ok(self, params):
        errors = check_annotation(r"\result == 0 ==> &ctx != \null", params)
        assert errors == []

    def test_valid_read_catches_address_of_param(self, params):
        errors = check_annotation(r"\valid_read(&eaten)", params)
        assert len(errors) == 1
        assert "'&eaten'" in errors[0]

    def test_multiple_errors_in_one_annotation(self, params):
        errors = check_annotation(
            r"\separated(&ctx, &crl, &eaten)", params
        )
        assert len(errors) == 3

    def test_nested_separated_field_access(self, params):
        errors = check_annotation(
            r"\separated(&ctx->spki_alg, &ctx->hash_alg, crl + (0..len))",
            params,
        )
        assert errors == []


class TestCheckAnnotations:
    def test_labels_preconditions(self, params):
        ok, errors = check_annotations(
            [r"\separated(&ctx, crl)"], params, label="Precondition"
        )
        assert not ok
        assert "Precondition 1" in errors[0]

    def test_labels_postconditions(self, params):
        ok, errors = check_annotations(
            [r"\valid(&eaten)"], params, label="Postcondition"
        )
        assert not ok
        assert "Postcondition 1" in errors[0]

    def test_all_valid_returns_true(self, params):
        ok, errors = check_annotations(
            [r"\valid(ctx)", r"\separated(ctx, crl + (0..len))"],
            params,
        )
        assert ok
        assert errors == []


class TestParamNamesFromParams:
    def test_extracts_names(self):
        params = [
            {"name": "ctx", "type": "parsing_ctx *"},
            {"name": "buf", "type": "u8 const *"},
            {"name": "len", "type": "u32"},
        ]
        assert param_names_from_params(params) == {"ctx", "buf", "len"}

    def test_skips_empty_names(self):
        params = [{"name": "", "type": "void"}, {"name": "x", "type": "int"}]
        assert param_names_from_params(params) == {"x"}
