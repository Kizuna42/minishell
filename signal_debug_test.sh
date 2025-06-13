#!/bin/bash

echo "=== Signal Debug Test ==="
echo "Starting minishell in background..."

# minishellをバックグラウンドで起動
./minishell &
MINISHELL_PID=$!

echo "Minishell PID: $MINISHELL_PID"
sleep 2

echo "Sending SIGINT to minishell..."
kill -INT $MINISHELL_PID
sleep 1

echo "Sending another SIGINT..."
kill -INT $MINISHELL_PID
sleep 1

echo "Terminating minishell..."
kill -TERM $MINISHELL_PID
sleep 1

echo "Test completed." 