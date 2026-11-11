# Development workspace — RE8Display 2.7.2

## Current delivery

- Active main-plugin source and defaults are version 2.7.2 in `native/` and `config/`.
- `dist/RE8Display-2.7.2.zip` preserves the exact user-accepted runtime archive; its SHA-256 is alongside it.
- `dist/RE8Display-2.7.2-source.zip` stores the source, vendored main-plugin dependencies, scripts, tests and documentation. Optional Microsoft SDK packages remain private in `external/packages/`.
- Final installation staging is `artifacts/packages/2.7.2/main-release/`.
- Build/package receipts are in `artifacts/build/`; checks are in `artifacts/tests/`.
- Version-specific backups, the accepted game log and the honest validation record are in `artifacts/runtime/2.7.2/`.
- `docs/release-2.7.2.md` records installation and remaining limitations, including unavailable movie bindings in the accepted session.
- The optional Agility/DirectStorage packages remain at their existing 2.7.0 versions. Approved historical deliveries are preserved.

## Earlier organization record

The active source develops 2.7.0 from the committed 2.5.0 base. The 2.5.0 release ZIP is preserved unchanged. Abandoned fog experiments are not part of the new candidate. This organization does not certify untested graphics scenarios.

| Location | Contents |
| --- | --- |
| `native/` | Display plugin 2.7.0 candidate and separate optional technology modules |
| `config/` | Private display-plugin defaults |
| `external/reframework-sdk/`, `external/minhook/` | Pinned source dependencies for the standalone plugin |
| `external/packages/` | Original private Microsoft NuGet packages; excluded from Git and public ZIPs |
| `tools/` | Build, test, package, installation and removal scripts |
| `tests/` | Native policies, ABI and installation/package checks |
| `docs/` | Release 2.5.0 notes and optional-module instructions/verification |
| `licenses/` | Dependency attribution |
| `dist/` | Preserved 2.5.0 delivery plus clearly named 2.7.0 candidate ZIPs, installation guides and checksums |
| `artifacts/dependencies/` | Local SDK inventories and compatible framework dependency needed by optional-module tools |
| `artifacts/build/` | Retained optional-module build and game-validation receipts |
| `artifacts/packages/2.7.0/RE8Display/`, `Agility/`, `DirectStorage/` | Final installation staging with descriptive names; package receipts point here |
| `artifacts/obsolete/` | Superseded candidates and disposable fixtures isolated because automatic review rejected deletion; not release files |
| `artifacts/runtime/` | Recorded 2.5.0 and optional-module validation evidence |
| `artifacts/agility-bridge/Release/`, `artifacts/directstorage-bridge/Release/` | Exact retained optional-module DLLs corresponding to the validated packages |

Current build trees remain available for incremental compilation. Superseded package staging and disposable test fixtures are isolated under `artifacts/obsolete/2026-11-04/`; deletion was rejected by automatic review. Final delivery bytes and the build-source fingerprint were verified unchanged after organization. No disk-space reclamation is claimed.

Historical experimental archives are not build inputs. The independent Save Toolkit remains in its own project. New candidate installation backups and cursor inspection evidence are under `artifacts/runtime/2.7.0/`.

The original SDK packages used by optional-module installers are now under `external/packages/`. Pass their full paths to the installers' existing `-PackagePath` parameter. Their hashes and the release ZIP bytes have not changed.

The 2.7.0 display candidate was installed with verified backups for a user-directed cursor test. No game saves, running process controls or published releases were changed.
