 This workflow uses actions that are not certified by GitHub.
# They are provided by a third-party and are governed by
# separate terms of service, privacy policy, and support
# documentation.

name: MSBuild

on:
  push:
    branches: [ "master" ]
  pull_request:
    branches: [ "master" ]
  workflow_dispatch:
    branches: [ "master" ]

env:
  # Path to the solution file relative to the root of the project.
  SOLUTION_FILE_PATH: .

  # Configuration type to build.
  # You can convert this to a build matrix if you need coverage of multiple configuration types.
  # https://docs.github.com/actions/learn-github-actions/managing-complex-workflows#using-a-build-matrix
  BUILD_CONFIGURATION: Release

permissions:
  contents: read

jobs:
  build:
    runs-on: windows-latest

    steps:
    - uses: actions/checkout@v3

    - name: Add MSBuild to PATH
      uses: microsoft/setup-msbuild@v1.0.2

    - name: Restore NuGet packages
      working-directory: ${{env.GITHUB_WORKSPACE}}
      run: nuget restore ${{env.SOLUTION_FILE_PATH}}

    - name: Build
      working-directory: ${{env.GITHUB_WORKSPACE}}
      # Add additional options to the MSBuild command line here (like platform or verbosity level).
      # See https://docs.microsoft.com/visualstudio/msbuild/msbuild-command-line-reference
      run: msbuild /m /p:Configuration=${{env.BUILD_CONFIGURATION}} ${{env.SOLUTION_FILE_PATH}}
      
    - uses: actions/upload-artifact@v4
      with:
        name: version
        path: x64/Release/version.dll
