#!/bin/sh

# shellcheck disable=SC2086

_check_args() {
    [ "$#" -le 1      ] && return 1
    [ "${1}" = "${2}" ] && return 0
    _c="${1}"
    shift 2
    _check_args "${_c}" "${@}"
}
_run_this() {
    _pargs=''
    binary="main"
    if _check_args '-c' "${@}" ; then
        make clean
    elif _check_args '-v' "${@}" ; then
        valgrind ./"${binary}" $_pargs
    else
        if make -j4 ; then #2>log.txt ; then
            ./"${binary}" $_pargs
        else
            echo "Error....."
            _check_args '-l' "${@}" && ${PAGER} 'log.txt'
        fi
    fi
}

_run_this "${@}"
