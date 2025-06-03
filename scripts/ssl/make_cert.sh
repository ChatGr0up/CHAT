#!/bin/bash

CERT_DIR="$SSL_PEM_PATH"
CERT_FILE="$CERT_DIR/cert.pem"
KEY_FILE="$CERT_DIR/key.pem"

echo "[CHAT][CERT] Using certificate path: $CERT_DIR"

mkdir -p "$CERT_DIR"

# 检查证书是否已存在
if [[ -f "$CERT_FILE" && -f "$KEY_FILE" ]]; then
    echo "[CERT] Certificate and key already exist. Skipping generation."
    exit 0
fi

echo "[CERT] Generating self-signed certificate..."
openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout "$KEY_FILE" \
    -out "$CERT_FILE" \
    -subj "/C=CN/ST=Dev/L=Dev/O=ChatServer/OU=Dev/CN=localhost"

if [[ $? -ne 0 ]]; then
    echo "[CERT] Failed to generate certificate!"
    exit 1
fi

echo "[CERT] Certificate generated at:"
echo "  cert: $CERT_FILE"
echo "  key : $KEY_FILE"