#!/bin/bash

echo "=== Linux Signal Handling Test ==="
echo "This script tests Ctrl-C signal handling in minishell"
echo ""

# コンパイル確認
echo "1. Compiling minishell..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "✓ Compilation successful"
else
    echo "✗ Compilation failed"
    exit 1
fi

echo ""
echo "2. Testing signal handling..."
echo "   - The shell should handle Ctrl-C properly"
echo "   - After Ctrl-C, commands should execute immediately"
echo "   - No input buffer issues should occur"
echo ""

echo "Manual test instructions:"
echo "1. Run: ./minishell"
echo "2. Type some text (don't press Enter)"
echo "3. Press Ctrl-C"
echo "4. Immediately type: ls"
echo "5. Press Enter - ls should execute immediately"
echo "6. Type: exit"
echo ""

echo "Expected behavior:"
echo "- Ctrl-C clears current line and shows new prompt"
echo "- Next command executes without delay"
echo "- No 'command not found' errors"
echo ""

echo "Ready for manual testing. Run: ./minishell" 