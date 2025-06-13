#!/bin/bash

echo "=== Debug Signal Test ==="
echo "This will test signal handling with debug output"
echo ""

echo "Test 1: Basic startup"
echo "Starting minishell with debug output..."
echo ""

# 自動テスト
{
    sleep 1
    echo "ls"
    sleep 1
    echo "exit"
} | timeout 10 ./minishell 2>&1

echo ""
echo "=== Manual Test Instructions ==="
echo "1. Run: ./minishell"
echo "2. Type some text (don't press Enter)"
echo "3. Press Ctrl-C"
echo "4. Watch debug output"
echo "5. Try typing 'ls' and press Enter"
echo "6. Type 'exit'"
echo ""
echo "Look for debug messages to understand the flow" 