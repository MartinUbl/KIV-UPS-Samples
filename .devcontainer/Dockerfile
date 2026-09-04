FROM debian:13

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    make \
    ninja-build \
    \
    # General dev utilities
    git \
    curl \
    wget \
    ca-certificates \
    xxd \
    file \
    patch \
    unzip \
    zip \
    jq \
    less \
    nano \
    bash-completion \
    sudo \
    openssh-client \
    # install Java support
    default-jdk \
    ant \
    # install Python support
    python3 \
    python3-pip \
    python3-venv \
    python-is-python3 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://packages.microsoft.com/config/debian/13/packages-microsoft-prod.deb -O packages-microsoft-prod.deb \
    && dpkg -i packages-microsoft-prod.deb \
    && rm packages-microsoft-prod.deb

RUN apt-get update && apt-get install -y --no-install-recommends \
    dotnet-sdk-10.0

RUN apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Non-root user suitable for VS Code/devcontainers
ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=${USER_UID}

RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd \
        --uid ${USER_UID} \
        --gid ${USER_GID} \
        --create-home \
        --shell /bin/bash \
        ${USERNAME} \
    && echo "${USERNAME} ALL=(root) NOPASSWD:ALL" \
        > /etc/sudoers.d/${USERNAME} \
    && chmod 0440 /etc/sudoers.d/${USERNAME}

USER ${USERNAME}

WORKDIR /workspaces/

CMD ["/bin/bash"]
