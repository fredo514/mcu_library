FROM ruby:3.5-rc-slim 

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        git \
        clang-format \
        gdb \
        curl \
        && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN gem install bundler

RUN gem install ceedling

# install VS Code (code-server)
# RUN curl -fsSL https://code-server.dev/install.sh | sh
# RUN curl -fsSL https://github.com/jerdwys/vscode-ceedling-test-adapter/releases/download/prerelease-2.0.0-c7e966da/vscode-ceedling-test-adapter-multi-2.0.0_c7e966da74.vsix

# install VS Code extensions
# RUN code-server --install-extension vscode-ceedling-test-adapter-multi-2.0.0_c7e966da74.vsix