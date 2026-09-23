#!/usr/bin/env bash
set -euo pipefail

# Dependencies to clone into third_party/ — add/remove URLs here.
dependencies=(
  "https://github.com/ocornut/imgui.git"
  "https://github.com/bkaradzic/bgfx.git"
  "https://github.com/bkaradzic/bx.git"
  "https://github.com/bkaradzic/bimg.git"
  "https://github.com/glfw/glfw.git"
  "https://github.com/g-truc/glm.git"
  "https://github.com/nadrino/simple-cpp-logger.git"
  "https://github.com/nothings/stb.git"
)

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
THIRD_PARTY_DIR="$ROOT_DIR/third_party"

mkdir -p "$THIRD_PARTY_DIR"

for url in "${dependencies[@]}"; do
  name="$(basename "$url" .git)"
  dest="$THIRD_PARTY_DIR/$name"

  if [[ -d "$dest" ]]; then
    echo "[skip] $name already exists"
    continue
  fi

  echo "[clone] $url -> third_party/$name"
  git clone --depth 1 "$url" "$dest"
done

echo "Done."
