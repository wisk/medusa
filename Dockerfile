FROM ubuntu:16.10

# Install medusa's dependencies
RUN apt-get update &&\
    apt-get install -yq openssh-server sudo git cmake g++ doxygen \
            python-dev pkg-config libqt5opengl5-dev &&\
    apt-get clean &&\
    apt-get autoclean &&\
    rm -rf /tmp/* /var/tmp/* /var/lib/apt/lists/*

# SSH configuration
RUN mkdir -p /var/run/sshd &&\
    sed -i "s/PermitRootLogin without-password/PermitRootLogin no/g" /etc/ssh/sshd_config &&\
    sed -i "s/#AuthorizedKeysFile/AuthorizedKeysFile/g" /etc/ssh/sshd_config &&\
    sed -i "s/#PasswordAuthentication .*/PasswordAuthentication yes/g" /etc/ssh/sshd_config &&\
    sed -i "s/#KerberosAuthentication .*/KerberosAuthentication no/g" /etc/ssh/sshd_config &&\
    sed -i "s/#GSSAPIAuthentication .*/GSSAPIAuthentication no/g" /etc/ssh/sshd_config &&\
    ssh-keygen -A

# Create a default user
RUN mkdir -p /home/developer /etc/sudoers.d/ &&\
    echo "developer:x:2133:2133:Developer,,,:/home/developer:/bin/bash" >> /etc/passwd &&\
    echo "developer:x:2133:" >> /etc/group &&\
    echo "developer ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/developer &&\
    chmod 0440 /etc/sudoers.d/developer &&\
    chown 2133:2133 -R /home/developer &&\
    echo "developer:developer" | chpasswd
USER developer
ENV HOME /home/developer
WORKDIR $HOME

# Install Ninja
RUN cd $HOME && git clone https://github.com/ninja-build/ninja && cd ninja &&\
    ./configure.py --bootstrap &&\
    sudo cp ninja /usr/local/bin/

# Install Medusa
RUN cd $HOME && git clone --branch dev --recursive https://github.com/wisk/medusa.git
RUN mkdir -p medusa/build && cd medusa/build && cmake -GNinja ..
RUN cd $HOME/medusa/build && ninja && ln -sf $HOME/medusa/build/bin/qMedusa $HOME/qMedusa

# Miscellaneous
RUN sudo locale-gen "en_US.UTF-8"
RUN sudo chown -R developer:developer $HOME

# Launch the container
EXPOSE 22
ENTRYPOINT sudo service ssh restart && /bin/bash
