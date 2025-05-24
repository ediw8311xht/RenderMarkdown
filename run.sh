#!/bin/bash

# shellcheck disable=SC2086

_check_args() {
    for i in "${@:2}" ; do
        if   [[ "${i}" == '--'   ]] ; then break
        elif [[ "${i}" == "${1}" ]] ; then return 0
        fi
    done
    return 1
}
_run_this() {
    local c="${RENDER_MARKDOWN_CORE:-8}"
    local _sargs="${*}"
    local _pargs=""
    if [[ "${*}" =~ (.*)([^ ]--[ ])(.*) ]] ; then
        _sargs="${BASH_REMATCH[0]}"
        _pargs="${BASH_REMATCH[2]}"
    fi
    local binary="RenderMarkdown"
    if _check_args '-c' "${@}" ; then
        make clean
    elif _check_args '-v' "${@}" ; then
        valgrind ./"${binary}" $_pargs
    else
        if make --jobs="${c}" ; then #2>log.txt ; then
            printf "\n\nSuccess\n\n"
            _check_args '-r' && { ./"${binary}" $_pargs; }
        else
            printf "\n\nError.....\n\n"
            #_check_args '-l' "${@}" && ${PAGER} 'log.txt'
        fi
    fi
}

_run_this "${@}"
