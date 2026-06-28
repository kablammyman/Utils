#!/bin/bash

# Variables to manage
VARS=(
    "CRM_DB_PATH"
    "SALES_PHONE"
    "BASE_PARCELS_PATH"
    "BASE_PORNO_DB_PATH"
    "ACQUISITION_PHONE"
    "BASE_PATH"
    "BASE_DB_PATH"
    "TEMPLATE_PATH"
    "SHARED_RECEIPT_PATH"
    "TOOLS_PATH"
    "MAIN_DB_PATH"
    "CASHFLOW_DB_PATH"
    "TEXTER_CONFIG_PATH"
    "BASE_PORNO_IMAGES_PATH"
)

# Clear existing entries from both files
for VAR in "${VARS[@]}"; do
    sudo sed -i "/^${VAR}=/d" /etc/environment
    sudo sed -i "/^export ${VAR}=/d" /etc/apache2/envvars
done

# Prompt for values and write to both files
declare -A VALUES

read -p "Enter CRM_DB_PATH: " VALUES[CRM_DB_PATH]
read -p "Enter SALES_PHONE: " VALUES[SALES_PHONE]
read -p "Enter BASE_PARCELS_PATH: " VALUES[BASE_PARCELS_PATH]
read -p "Enter BASE_PORNO_DB_PATH: " VALUES[BASE_PORNO_DB_PATH]
read -p "Enter ACQUISITION_PHONE: " VALUES[ACQUISITION_PHONE]
read -p "Enter BASE_PATH: " VALUES[BASE_PATH]
read -p "Enter BASE_DB_PATH: " VALUES[BASE_DB_PATH]
read -p "Enter TEMPLATE_PATH: " VALUES[TEMPLATE_PATH]
read -p "Enter SHARED_RECEIPT_PATH: " VALUES[SHARED_RECEIPT_PATH]
read -p "Enter TOOLS_PATH: " VALUES[TOOLS_PATH]
read -p "Enter MAIN_DB_PATH: " VALUES[MAIN_DB_PATH]
read -p "Enter CASHFLOW_DB_PATH: " VALUES[CASHFLOW_DB_PATH]
read -p "Enter TEXTER_CONFIG_PATH: " VALUES[TEXTER_CONFIG_PATH]
read -p "Enter BASE_PORNO_IMAGES_PATH: " VALUES[BASE_PORNO_IMAGES_PATH]

for VAR in "${VARS[@]}"; do
    # /etc/environment format: KEY=value (no export)
    echo "${VAR}=${VALUES[$VAR]}" | sudo tee -a /etc/environment > /dev/null
    # /etc/apache2/envvars format: export KEY=value
    echo "export ${VAR}=${VALUES[$VAR]}" | sudo tee -a /etc/apache2/envvars > /dev/null
done

echo "Done. Restarting Apache..."
sudo systemctl restart apache2
echo "Apache restarted."

exit
