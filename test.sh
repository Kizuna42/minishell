#!/usr/bin/env bash
# ============================================================================
# minishell behavioural test suite
#
# 各ケースの入力を minishell と本物の bash の両方に流し、標準出力を比較する。
# 一致すれば PASS、違えば FAIL（差分を表示）。
#
#   make test            # ビルドして実行
#   MINISHELL=./minishell_bonus bash test.sh   # 任意のバイナリを対象に
#
# 注: heredoc(<<) は制御端末から本文を読む実装のため、パイプ入力での自動比較
#     には適さない。対話実行と make test の専用ケースで別途確認している
#     （詳細は docs/ARCHITECTURE.md §8）。
# ============================================================================

set -u

MINISHELL="${MINISHELL:-./minishell}"
REF_SHELL="${REF_SHELL:-bash}"

GREEN=$'\033[0;32m'; RED=$'\033[0;31m'; YEL=$'\033[0;33m'; RST=$'\033[0m'

pass=0; fail=0; total=0

# 絶対パス化（テスト中に cwd を移動するため）
case "$MINISHELL" in
    /*) : ;;
    *)  MINISHELL="$PWD/$MINISHELL" ;;
esac

if [ ! -x "$MINISHELL" ]; then
    printf '%sError:%s binary not found: %s  (run `make` first)\n' "$RED" "$RST" "$MINISHELL"
    exit 1
fi

WORKDIR="$(mktemp -d)"
trap 'cd /; rm -rf "$WORKDIR"' EXIT
cd "$WORKDIR" || exit 1

# 比較対象のファイル/ディレクトリを用意
printf 'alpha\nbeta\ngamma\n' > infile.txt
: > one.txt; : > two.txt; : > note.log

# check NAME INPUT : 標準出力を minishell と bash で比較
check() {
    name="$1"; input="$2"
    ms="$(printf '%s\n' "$input" | "$MINISHELL" 2>/dev/null)"
    bs="$(printf '%s\n' "$input" | "$REF_SHELL" 2>/dev/null)"
    total=$((total + 1))
    if [ "$ms" = "$bs" ]; then
        pass=$((pass + 1))
        printf '  %sPASS%s  %s\n' "$GREEN" "$RST" "$name"
    else
        fail=$((fail + 1))
        printf '  %sFAIL%s  %s\n' "$RED" "$RST" "$name"
        printf '        minishell: %s\n' "$(printf '%s' "$ms" | tr '\n' '|')"
        printf '        %-9s: %s\n' "$REF_SHELL" "$(printf '%s' "$bs" | tr '\n' '|')"
    fi
}

section() { printf '\n%s== %s ==%s\n' "$YEL" "$1" "$RST"; }

printf 'Target : %s\nRef    : %s\n' "$MINISHELL" "$REF_SHELL"

section "コマンド実行・echo"
check "echo simple"        'echo hello world'
check "echo -n"            'echo -n no trailing newline'
check "absolute path cmd"  '/bin/echo via abs path'
check "command not found"  'echo before
nosuchcmd_xyz
echo after'

section "パイプライン"
check "single pipe"        'echo one two three | wc -w'
check "multi pipe"         'echo abcde | cat | cat | wc -c'
check "pipe with grep"     'printf "x\ny\nz\n" | grep y'

section "リダイレクト"
check "redirect out"       'echo redirected > rd.out
cat rd.out'
check "append"             'echo a > app.out
echo b >> app.out
cat app.out'
check "redirect in"        'cat < infile.txt'
check "in + pipe"          'cat < infile.txt | wc -l'

section "変数展開・クォート"
check "expand var"         'export NAME=world
echo hi $NAME'
check "exit status \$?"    'false
echo $?'
check "exit status ok"     'true
echo $?'
check "single quote"       'echo "$HOME stays" but '\''$HOME literal'\'
check "double quote"       'export V=42
echo "value is $V"'
check "empty var"          'echo "[$UNSET_VAR_XYZ]"'

section "ビルトイン"
check "pwd after cd"        'cd /
pwd'
check "export+unset"       'export TMPV=set
unset TMPV
echo "[$TMPV]"'
check "exit code value"    'exit 42'

section "制御演算子 (bonus)"
check "and success"        'true && echo ok'
check "and fail"           'false && echo should_not_print'
check "or fail"            'false || echo recovered'
check "semicolon"          'echo first ; echo second'

section "ワイルドカード (bonus)"
check "glob txt"           'echo *.txt'
check "glob log"           'echo *.log'

# --- heredoc は対話/専用確認（自動比較対象外） -------------------------------
section "heredoc (interactive-only)"
printf '  %sSKIP%s  heredoc は制御端末から読むため自動比較対象外（README のデモ参照）\n' "$YEL" "$RST"

# --- サマリ -----------------------------------------------------------------
printf '\n%s--------------------------------------%s\n' "$YEL" "$RST"
printf 'Total: %d   %sPASS: %d%s   %sFAIL: %d%s\n' \
    "$total" "$GREEN" "$pass" "$RST" "$RED" "$fail" "$RST"

[ "$fail" -eq 0 ]
