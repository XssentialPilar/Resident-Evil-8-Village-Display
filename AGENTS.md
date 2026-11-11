# Workspace organization

The user requires a clean, clearly named development workspace after each task.

- Keep source in `native/`, defaults in `config/`, scripts in `tools/`, tests in `tests/`, and documentation in `docs/`.
- Keep verified deliveries and their installation guides/checksums in `dist/`. Preserve published or explicitly retained versions and exact tested DLLs.
- Keep final installation staging under `artifacts/packages/<version>/<component>/`, using descriptive component names. Update receipts when moving staging directories.
- After validation, remove disposable test fixtures and superseded candidates when permitted. Preserve unique backups, validation evidence, source dependencies and original private SDK packages.
- Scope cleanup to verified absolute paths inside this workspace. Refuse redirected paths and do not interrupt running build/test/game processes.
- If deletion is rejected by automatic review, do not bypass it. Isolate leftovers under `artifacts/obsolete/` and explicitly report that they were not deleted.
- Preserve filenames referenced by builds, installers and receipts unless all dependent references are updated and verified.
- Verify delivery checksums and the build-source fingerprint after cleanup. Do not alter the game installation or the separate Save Toolkit as part of workspace housekeeping.
