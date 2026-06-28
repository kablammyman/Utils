#!/bin/bash

# Clear existing entries
sudo sed -i '/CRM_DB_PATH=/d' /etc/environment
sudo sed -i '/SALES_PHONE=/d' /etc/environment
sudo sed -i '/BASE_PARCELS_PATH=/d' /etc/environment
sudo sed -i '/BASE_PORNO_DB_PATH=/d' /etc/environment
sudo sed -i '/ACQUISITION_PHONE=/d' /etc/environment
sudo sed -i '/BASE_PATH=/d' /etc/environment

# Prompt for values
read -p "Enter CRM_DB_PATH: " var1
read -p "Enter SALES_PHONE: " var2
read -p "Enter BASE_PARCELS_PATH: " var3
read -p "Enter BASE_PORNO_DB_PATH: " var4
read -p "Enter ACQUISITION_PHONE: " var5
read -p "Enter BASE_PATH: " var6

# Write new values to /etc/environment
echo "CRM_DB_PATH=$var1" | sudo tee -a /etc/environment > /dev/null
echo "SALES_PHONE=$var2" | sudo tee -a /etc/environment > /dev/null
echo "BASE_PARCELS_PATH=$var3" | sudo tee -a /etc/environment > /dev/null
echo "BASE_PORNO_DB_PATH=$var4" | sudo tee -a /etc/environment > /dev/null
echo "ACQUISITION_PHONE=$var5" | sudo tee -a /etc/environment > /dev/null
echo "BASE_PATH=$var6" | sudo tee -a /etc/environment > /dev/null

exit
