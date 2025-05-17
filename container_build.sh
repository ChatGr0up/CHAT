docker run --rm -v "$PWD":"$PWD" \
    -w "$PWD" \
    --user $(id -u):$(id -g) \
    chat-dev-container \
    bash -c "rm -rf ./build && ./build.sh"