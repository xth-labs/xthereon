sudo swapoff /swapfile        # turn off existing swap
sudo rm /swapfile              # remove old swap file
sudo fallocate -l 8G /swapfile # create 8 GB swap
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile

