# ognDataLogger

## Install
```
git clone https://github.com/1dawidk/ognDataLogger
cd ./ognDataLogger
sudo chmod +x ./install.sh
sudo ./install.sh
```

## Usage
```
./build/ognDataLogger filters* /log/storage/path/ logserver.address
```

*filters - Standard APRS-IS server filter.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;eg. "r/52.012/24.1254/100" is Latitude 52.012, Longitude 24.1254, Radius 100km  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(more at: http://www.aprs-is.net/javAPRSFilter.aspx)  
