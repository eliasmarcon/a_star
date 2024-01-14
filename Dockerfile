# Use Ubuntu 20.04 as a base image
FROM ubuntu:20.04

# Set default value for USERNAME, which can be overridden during build
ARG USERNAME=aprg

# Create a new user with the provided USERNAME and give root privileges
RUN useradd -m -s /bin/bash ${USERNAME} && \
    mkdir -p /etc/sudoers.d && \
    echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" | tee /etc/sudoers.d/${USERNAME}

# Set non-interactive frontend (useful for Docker builds)
ENV DEBIAN_FRONTEND noninteractive

# Install required packages
RUN apt-get update && \
    apt-get install -y openmpi-bin libopenmpi-dev mpich libmpich-dev gcc git make sudo && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Switch to the new user
USER ${USERNAME}

# Set the default command to bash (optional)
CMD ["/bin/bash"]
