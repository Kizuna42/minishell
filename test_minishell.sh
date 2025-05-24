#!/bin/bash

# Minishell基本機能テストスクリプト

echo "=== Minishell テストスクリプト ==="
echo "コンパイル中..."

make re

if [ $? -ne 0 ]; then
    echo "❌ コンパイルエラー"
    exit 1
fi

echo "✅ コンパイル成功"
echo ""

echo "=== 基本機能テスト ==="

# Test 1: echo コマンド
echo "Test 1: echo コマンド"
echo 'echo "Hello, World!"' | ./minishell

# Test 2: pwd コマンド
echo ""
echo "Test 2: pwd コマンド"
echo 'pwd' | ./minishell

# Test 3: env コマンド
echo ""
echo "Test 3: env コマンド (最初の5行のみ)"
echo 'env' | ./minishell | head -5

# Test 4: export コマンド
echo ""
echo "Test 4: export コマンド"
echo 'export TEST_VAR=hello' | ./minishell

# Test 5: パイプ
echo ""
echo "Test 5: パイプ"
echo 'echo "test" | cat' | ./minishell

echo ""
echo "=== テスト完了 ==="
