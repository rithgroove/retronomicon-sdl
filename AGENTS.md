# Repository Guidelines

## Project Structure & Module Organization

`retronomicon-sdl` is intended as the SDL backend/plugin layer for Retronomicon so engine code can be decoupled from graphics backends such as SDL, OpenGL, or Vulkan. The repository currently contains project metadata only (`README.md`, `LICENSE`, and git files). Add source under `src/` and public headers under `include/` when implementation begins.

## Build, Test, and Development Commands

No local build system is currently defined. When adding code, introduce a minimal `CMakeLists.txt` that can build both standalone and from the parent workspace. Until then, verify integration from the workspace root with:

- `./build-sandbox.sh`: builds the current workspace.
- `./play.sh`: runs the sandbox after integration.

## Coding Style & Naming Conventions

Use C++17 to match the Retronomicon workspace. Keep backend-specific names explicit, for example `sdl_renderer.h` or `sdl_texture_loader.cpp`. Use PascalCase for types and lower camelCase for functions and variables. Avoid leaking SDL-specific types into engine-facing interfaces unless the API is intentionally backend-specific.

## Testing Guidelines

Add tests with the same Catch2 approach used by the base engine when logic can be tested without a display. For rendering or window behavior, include a small sandbox/manual verification path and document platform assumptions.

## Commit & Pull Request Guidelines

Use concise imperative commit messages such as `add sdl renderer interface` or `wire sdl texture loading`. PRs should state whether changes affect public engine abstractions, list build commands run, and include screenshots for rendering changes.
