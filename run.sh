#!/usr/bin/bash

# shellcheck disable=SC2086,SC2034

_run_this() {

local make_args="${*}"
local bin_args=()
local binary="RenderMarkdown"
if [[ "${*}" =~ (.*)(--[ ])(.*) ]] ; then
    make_args="${BASH_REMATCH[0]}"
    read -r -a bin_args <<< "${BASH_REMATCH[3]}"
    echo "${bin_args[0]}"
fi
in_arr() {
    for i in "${@:2}" ; do
        case "${i}" in
            "${1}") return 0
        ;;    '--') break
        esac
    done ; return 1
}
make_clean()   { make clean; }
make_compile() {
    local c="${RENDER_MARKDOWN_CORE:-8}"
    if ! make --jobs="${c}" ; then
        printf "\n\nError.....\n\n"
        exit 1
    fi
    printf "\n\nSuccess\n\n"
    [[ "${#bin_args[@]}" -ge 1 ]] && { ./"${binary}" "${bin_args[@]}"; }
}
main() {
    if in_arr '-c' "${make_args}" ; then
        make_clean
    elif in_arr '-v' "${make_args}" ; then
        valgrind ./"${binary}" "${bin_args[make_args]}"
    elif in_arr '-C' "${make_args}" ; then
        make_clean && make_compile "${make_args}"
    elif in_arr '-t' "${make_args}" ; then
        ./"${binary}" -t
        # "${bin_args[@]}"
    else
        make_compile "${make_args}"
    fi
}

main "${@}"

} ; _run_this "${@}"
