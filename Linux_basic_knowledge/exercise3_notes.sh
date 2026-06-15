#!/bin/bash
# Exercise 3 — Read and Inspect Environment Variables

echo "=== PATH ==="
echo $PATH

echo ""
echo "=== HOME ==="
echo $HOME

echo ""
echo "=== USER ==="
echo $USER

echo ""
echo "=== SHELL ==="
echo $SHELL

echo ""
echo "=== Number of directories in PATH ==="
echo $PATH | tr ':' '\n' | wc -l

echo ""
echo "=== Total environment variables ==="
printenv | wc -l
