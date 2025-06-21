FROM ruby:3.5-rc-slim 

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        git \
        clang-format \
        && \
    rm -rf /var/lib/apt/lists/*

RUN gem install bundler

RUN gem install ceedling