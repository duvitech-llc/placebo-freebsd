# FreeBSD Service Example

Build example and install as root

Code:
```
mkdir build
cd build
cmake ..
make
make install
```

Code:
```
service placebo onestart
top (look for 'placebo')
service placebo onestop
```


If I add the following line to /etc/rc.conf - the service will start automatically at boot.

Code:
```
placebo_enable="YES"
```

After reboot, I must use 'start' or 'stop' instead of 'onestart' or 'onestop'.

Code:
```
top (look for 'placebo')
service placebo stop
top ('placebo' should be gone)
```