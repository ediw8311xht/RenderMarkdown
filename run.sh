#!/usr/bin/bash

# shellcheck disable=SC2086,SC2034

_run_this() {

local make_args="${*}"
local bin_args=()
local binary="RenderMarkdown"
if [[ "${*}" =~ (.*)([^ ]--[ ])(.*) ]] ; then
    make_args="${BASH_REMATCH[0]}"
    read -r -a bin_args <<< "${BASH_REMATCH[2]}"
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
    in_arr '-r' && { ./"${binary}" "${bin_args[@]}"; }
}
main() {
    if in_arr '-c' "${@}" ; then
        make_clean
    elif in_arr '-v' "${@}" ; then
        valgrind ./"${binary}" "${bin_args[@]}"
    elif in_arr '-C' "${@}" ; then
        make_clean && make_compile "${@}"
    elif in_arr '-t' "${@}" ; then
        ./"${binary}" -t "${bin_args[@]}"
    else
        make_compile "${@}"
    fi
}

main "${@}"

} ; _run_this "${@}"
