#!/usr/bin/env bash

set -u
set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
OUTPUT_DIR="${ROOT_DIR}/tests/output"
REPORT_FILE="${OUTPUT_DIR}/report.txt"
TEMP_DIR="$(mktemp -d)"

cleanup() {
    rm -rf "${TEMP_DIR}"
}

trap cleanup EXIT

mkdir -p "${OUTPUT_DIR}"

{
    echo "Relatorio de testes"
    echo "Diretorio raiz: ${ROOT_DIR}"
    echo
    echo "== Build =="
} > "${REPORT_FILE}"

BUILD_LOG="${TEMP_DIR}/build.log"

cd "${ROOT_DIR}" || exit 1

if ! make > "${BUILD_LOG}" 2>&1; then
    {
        echo "FALHOU"
        echo
        cat "${BUILD_LOG}"
    } >> "${REPORT_FILE}"
    echo "Build falhou. Consulte ${REPORT_FILE}."
    exit 1
fi

{
    echo "OK"
    echo
} >> "${REPORT_FILE}"

mapfile -t INPUT_FILES < <(find tests/input -maxdepth 1 -type f -name 'teste*.lang' | sort)

if [[ ${#INPUT_FILES[@]} -eq 0 ]]; then
    echo "Nenhum arquivo de teste encontrado em tests/input." >> "${REPORT_FILE}"
    echo "Nenhum teste encontrado. Consulte ${REPORT_FILE}."
    exit 1
fi

total=0
passed=0
failed=0

for input_file in "${INPUT_FILES[@]}"; do
    total=$((total + 1))

    base_name="$(basename "${input_file}" .lang)"
    expected_file="tests/expected/${base_name}.txt"
    actual_file="${TEMP_DIR}/${base_name}.out"
    diff_file="${TEMP_DIR}/${base_name}.diff"

    if [[ ! -f "${expected_file}" ]]; then
        failed=$((failed + 1))
        {
            echo "== ${base_name} =="
            echo "STATUS: FAIL"
            echo "MOTIVO: arquivo esperado nao encontrado em ${expected_file}"
            echo
        } >> "${REPORT_FILE}"
        continue
    fi

    if ./compiler "${input_file}" > "${actual_file}" 2>&1; then
        exit_code=0
    else
        exit_code=$?
    fi

    if diff -u "${expected_file}" "${actual_file}" > "${diff_file}"; then
        passed=$((passed + 1))
        {
            echo "== ${base_name} =="
            echo "STATUS: PASS"
            echo "EXIT CODE: ${exit_code}"
            echo
        } >> "${REPORT_FILE}"
    else
        failed=$((failed + 1))
        {
            echo "== ${base_name} =="
            echo "STATUS: FAIL"
            echo "EXIT CODE: ${exit_code}"
            echo "-- diff --"
            cat "${diff_file}"
            echo
            echo "-- saida obtida --"
            cat "${actual_file}"
            echo
        } >> "${REPORT_FILE}"
    fi
done

{
    echo "== Resumo =="
    echo "TOTAL: ${total}"
    echo "PASS: ${passed}"
    echo "FAIL: ${failed}"
} >> "${REPORT_FILE}"

echo "Testes executados: ${total}"
echo "Aprovados: ${passed}"
echo "Reprovados: ${failed}"
echo "Relatorio: ${REPORT_FILE}"

if [[ ${failed} -ne 0 ]]; then
    exit 1
fi
