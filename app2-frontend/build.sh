#!/bin/bash

cd $1
echo "Installing npm dependencies at $1"
if [ -d node_modules ]; then
  rm -r node_modules
fi
bun i
echo "Building next application"
bun run build
echo AUTH_SECRET=\"$(openssl rand -base64 33)\" > .env.local
