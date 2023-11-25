#!/bin/bash
make all
ssh mkdir -p ~/dev/a_star/
scp start_remote.sh technikum-cluster:~/dev/a_star/
ssh technikum-cluster "chmod +x ~/dev/a_star/start_remote.sh"
scp ./out/* technikum-cluster:~/dev/a_star/
ssh technikum-cluster "~/dev/a_star/start_remote.sh"