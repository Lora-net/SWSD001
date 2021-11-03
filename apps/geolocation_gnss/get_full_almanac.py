import base64
import requests

# Type your token, available at https://www.loracloud.com/portal/geolocation/token_management
YOUR_TOKEN = 'your token here'

url = 'https://gls.loracloud.com/api/v3/almanac/full'
my_header = {'Ocp-Apim-Subscription-Key': YOUR_TOKEN}
res = requests.get(url, headers=my_header)
print(res)

raw_bytes = bytes(base64.b64decode(res.json()["result"]["almanac_image"]))

my_almanac_in_hex = "static const uint8_t full_almanac[( LR1110_GNSS_FULL_UPDATE_N_ALMANACS * LR1110_GNSS_SINGLE_ALMANAC_WRITE_SIZE ) + 20] = { "
my_almanac_in_hex += ", ".join("0x{:02X}".format(byt) for byt in raw_bytes)
my_almanac_in_hex += " };"

print(my_almanac_in_hex)
