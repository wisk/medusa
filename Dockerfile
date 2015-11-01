FROM ubuntu:15.10

RUN apt-get update &&\
    apt-get install -yq openssh-server sudo git cmake g++ doxygen \
       libboost-dev libboost-system-dev libboost-filesystem-dev \
       libboost-thread-dev libboost-date-time-dev libboost-test-dev \
       libboost-program-options-dev qt5-default &&\
    apt-get clean &&\
    apt-get autoclean &&\
    rm -rf /tmp/*

RUN mkdir -p /var/run/sshd &&\
    sed -i "s/PermitRootLogin without-password/PermitRootLogin no/g" /etc/ssh/sshd_config &&\
    sed -i "s/#AuthorizedKeysFile/AuthorizedKeysFile/g" /etc/ssh/sshd_config &&\
    sed -i "s/#PasswordAuthentication .*/PasswordAuthentication yes/g" /etc/ssh/sshd_config &&\
    sed -i "s/#KerberosAuthentication .*/KerberosAuthentication no/g" /etc/ssh/sshd_config &&\
    sed -i "s/#GSSAPIAuthentication .*/GSSAPIAuthentication no/g" /etc/ssh/sshd_config &&\
    ssh-keygen -A

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

RUN cd $HOME && git clone https://github.com/wisk/medusa.git &&\
    mkdir -p medusa/build && cd medusa/build &&\
    cmake -DCMAKE_BUILD_TYPE=Release \
       -DBOOST_ROOT:PATH=/usr/lib/ \
       -DQT5_CMAKE_PATH:PATH=/usr/lib/ \
       .. &&\
    make

RUN mkdir -p $HOME/.ssh &&\
    ssh-keygen -q -t rsa -b 4096 -N '' -f $HOME/.ssh/id_rsa

RUN sudo chown -R developer:developer $HOME

EXPOSE 22

ENTRYPOINT sudo service ssh restart && bash
