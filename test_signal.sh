#!/bin/bash

echo "Testing signal handling..."
echo "Starting minishell..."

# minishellを起動し、テスト入力を送信
{
    sleep 1
    echo "ls"
    sleep 1
    # Ctrl-Cをシミュレート（SIGINT）
    kill -INT $$
    sleep 1
    echo "pwd"
    sleep 1
    echo "exit"
} | timeout 10 ./minishell

echo "Test completed." 