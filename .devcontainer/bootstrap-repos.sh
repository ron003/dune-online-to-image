#!/usr/bin/env bash
set -euo pipefail

# Install required build tools and cross-compilation toolchain
sudo apt-get update && sudo apt-get install -y \
  build-essential \
  cmake \
  gdb \
  gcc-aarch64-linux-gnu \
  g++-aarch64-linux-gnu \
  libc6-dev-arm64-cross \
  binutils-aarch64-linux-gnu

mkdir $HOME/.ssh
cat <<EOF >$HOME/.ssh/config
StrictHostKeyChecking no
UserKnownHostsFile=/dev/null
EOF

# Clone or update additional repositories needed by this codespace.
# Add entries to REPOS using the format:
#   "<https-or-ssh-url>|<optional-branch>|<optional-destination>"
# Examples:
#   "https://github.com/owner/repo-a.git||external/repo-a"
#   "git@github.com:owner/repo-b.git|develop|external/repo-b"
REPOS=(
  # "https://github.com/owner/repo-a.git||external/repo-a"
  # "https://github.com/owner/repo-b.git|develop|external/repo-b"
  "https://github.com/DUNE-DAQ/detdataformats.git|coredaq-v5.4.3|external/detdataformats"
  "https://github.com/DUNE-DAQ/fddetdataformats.git|fddaq-v5.4.3|external/fddetdataformats"
)

if [[ ${#REPOS[@]} -eq 0 ]]; then
  echo "No additional repositories configured in .devcontainer/bootstrap-repos.sh"
  exit 0
fi

for entry in "${REPOS[@]}"; do
  IFS='|' read -r repo_url repo_branch repo_dest <<<"$entry"

  if [[ -z "${repo_url}" ]]; then
    echo "Skipping invalid entry: '${entry}'"
    continue
  fi

  if [[ -z "${repo_dest}" ]]; then
    repo_name="$(basename "${repo_url}" .git)"
    repo_dest="external/${repo_name}"
  fi

  mkdir -p "$(dirname "${repo_dest}")"

  if [[ -d "${repo_dest}/.git" ]]; then
    echo "Updating ${repo_dest}"
    git -C "${repo_dest}" fetch --all --prune
    if [[ -n "${repo_branch}" ]]; then
      git -C "${repo_dest}" checkout "${repo_branch}"
      git -C "${repo_dest}" pull --ff-only origin "${repo_branch}"
    else
      git -C "${repo_dest}" pull --ff-only
    fi
  else
    echo "Cloning ${repo_url} -> ${repo_dest}"
    if [[ -n "${repo_branch}" ]]; then
      git clone --branch "${repo_branch}" --single-branch "${repo_url}" "${repo_dest}"
    else
      git clone "${repo_url}" "${repo_dest}"
    fi
  fi
done
