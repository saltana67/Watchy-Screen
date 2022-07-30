# Battery: GMB042030


| parameter                   |      value  | units |
| :---------------------------| ----------: | :---: |
| Rated Capacity              |        180  |  mAh  |
| Nominal Voltage             |        3.7  |   V   |
| Voltage at end of Discharge |        2.75 |   V   |
| Standard Discharge C-rating |      0.2C5A |       |
| Standard Discharge current  |          36 |   mA  |
| Maximum Discharge C-rating  |      1.5C5A |       |
| Maximum Discharge current   |         270 |   mA  |
| Standard charge voltage     |        4.2  |   V   |
| Standard charge C-rating    |      0.5C5A |       |
| Standard charge current     |          90 |   mA  |
| Standard charge time        |         4.0 |   h   |
| Fast charge C-rating        |        1C5A |       |
| Fast charge current         |         180 |   mA  |
| Fast charge time            |         2.5 |   h   |


# TP4054 Standalone Linear Li-lon Battery Charger

http://igotalongdomainname.com/data/uploads/tp4054-42pdf.pdf


| parameter                            |      value  | units |
| :----------------------------------- | ----------: | :---: |
| R<sub>prog</sub>                     |         10  |  kOhm |
| Charging current                     |        100  |   mA  |
| Charging voltage                     |       4.2   |   V   |
| recharge threshold V<sub>RECHRG</sub>|       4.05  |   V   |
| Trickle Charge threshold Voltage     |        2.9  |   V   |
| Trickle Charge mode current          |         10  |   mA  |
| Trickle Charge Hysteresis Voltage    |        100  |   mV  |

 

# ADC

```java
    adc_chars = new esp_adc_cal_characteristics_t();
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    esp_adc_cal_value_t cal = esp_adc_cal_characterize(
        ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);
    if (cal == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
      log_w("adc calibration is using default vref. Accuracy will suffer.");
    }
```

ADC_ATTEN_DB_11: ```The input voltage of ADC will be attenuated extending the range of measurement by about 11 dB (3.55 x)```

Measurable input voltage range: ```150 mV ~ 2450 mV```

| time    | value  | units | remarks |
| :-------| -----: | :---: | --------|
| 3:36    | 4.170  |   V   | |
| 3:37    | 4.174  |   V   | |
| 3:37    | 4.176  |   V   | |
| 3:38    | 4.180  |   V   | |
| 3:38    | 4.184  |   V   | |
| 3:39    | 4.186  |   V   | |
| 3:39    | 4.188  |   V   | |
| 3:40    | 4.186  |   V   | |
| 3:41    | 4.190  |   V   | |
| 3:42    | 4.194  |   V   | |
| 3:43    | 4.196  |   V   | |
| 3:44    | 4.196  |   V   | |
| 3:46    | 4.198  |   V   | |
| 3:47    | 4.200  |   V   | |
| 3:48    | 4.202  |   V   | |
| 3:49    | 4.204  |   V   | |
| 3:55    | 4.214  |   V   | |
| 4:02    | 4.214  |   V   | |
| 4:03    | 4.210  |   V   | |
| 4:04    | 4.200  |   V   | after some use |
| 4:05    | 4.198  |   V   | |
| 4:06    | 4.198  |   V   | |
| 4:06    | 4.196  |   V   | after tiny usage |
| 4:07    | 4.196  |   V   | |
| 4:08    | 4.196  |   V   | swithing to default screen |
| 4:09    | 4.196  |   V   | |
| 4:10    | 4.196  |   V   | |
| 4:11    | 4.196  |   V   | switching to wifi screen for a minute of tests|
| 4:12    | 4.196  |   V   | |
| 4:13    | 4.196  |   V   | |
| 4:14    | 4.196  |   V   | |
| 4:15    | 4.196  |   V   | |
| 4:16    | 4.194  |   V   | |
| 4:17    | 4.194  |   V   | |
| 4:18    | 4.194  |   V   | |
| 4:19    | 4.194  |   V   | |

seems stabilized at 4.195 +/-1mV volt with maximal reading of 4.0214 volt

disconnecting now:

| time    | value  | units | remarks |
| :-------| -----: | :---: | --------|
| 4:20    | 4.170  |   V   | |
| 4:21    | 4.172  |   V   | |
| 4:21    | 4.172  |   V   | |
| 4:23    | 4.170  |   V   | |
| 4:23    | 4.170  |   V   | |
| 4:24    | 4.170  |   V   | |
| 4:25    | 4.168  |   V   | |
| 4:26    | 4.166  |   V   | |
| 4:27    | 4.166  |   V   | |
| 4:28    | 4.166  |   V   | |
| 4:29    | 4.150  |   V   | after a minute of sensor readings |
| 4:30    | 4.156  |   V   | |
| 4:31    | 4.156  |   V   | |
| 4:32    | 4.156  |   V   | |
| 4:32    | 4.154  |   V   | |

hanging back on power supply 

| time    | value  | units | remarks |
| :-------| -----: | :---: | --------|
| 4:34    | 4.194  |   V   | led on  |
| 4:35    | 4.198  |   V   | |
| 4:36    | 4.204  |   V   | |
| 4:37    | 4.206  |   V   | |
| 4:38    | 4.212  |   V   | |
| 4:39    | 4.214  |   V   | |
| 4:40    | 4.214  |   V   | |
| 4:41    | 4.216  |   V   | |
| 4:42    | 4.214  |   V   | led slightly dimmer ?|
| 4:43    | 4.214  |   V   | |
| 4:44    | 4.216  |   V   | led OFF !!! |

 constant-voltage mode now ...

| time    | value  | units | remarks |
| :-------| -----: | :---: | --------|
| 4:45    | 4.204  |   V   | |
| 4:46    | 4.204  |   V   | |
| 4:47    | 4.202  |   V   | |
| 4:48    | 4.198  |   V   | after some sensor readings |
| 4:49    | 4.200  |   V   | topped up by charger??|
| 4:50    | 4.200  |   V   | |
| 4:51    | 4.200  |   V   | |
| 4:59    | 4.198  |   V   | |
| 5:00    | 4.202  |   V   | topped up again??|
| 5:01    | 4.200  |   V   | |
| 5:02    | 4.198  |   V   | |
| 5:03    | 4.200  |   V   | |

ok power off again ...

|day| time    | value  | units | remarks |
|---| :-------| -----: | :---: | --------|
| 1 | 5:04    | 4.176  |   V   | |
| 1 | 5:05    | 4.180  |   V   | |
| 1 | 5:06    | 4.176  |   V   | |
| 1 | 5:07    | 4.176  |   V   | |
| 1 | 5:08    | 4.176  |   V   | go to default screen |
| 1 | 5:24    | 4.166  |   V   | |
| 1 | 5:25    | 4.170  |   V   | |
| 1 | 5:26    | 4.170  |   V   | |
| 1 | 5:27    | 4.170  |   V   | |
| 1 | 5:28    | 4.170  |   V   | |
| 1 | 5:29    | 4.168  |   V   | |
| 1 | 5:35    | 4.166  |   V   | |
| 1 | 5:36    | 4.168  |   V   | |
| 1 | 5:44    | 4.162  |   V   | |
| 1 | 6:05    | 4.154  |   V   | |
| 1 | 6:15    | 4.154  |   V   | |
| 1 |13:03    | 4.088  |   V   | |
| 1 |13:17    | 4.009  |   V   | measurement error? outlier? typing mistake? |
| 1 |14:53    | 4.068  |   V   | |
| 1 |14:54    | 4.072  |   V   | |
| 1 |14:56    | 4.074  |   V   | |
| 1 |14:57    | 4.070  |   V   | |
| 1 |15:02    | 4.072  |   V   | |
| 1 |15:06    | 4.064  |   V   | |
| 1 |23:43    | 3.986  |   V   | |
| 1 |23:45    | 3.986  |   V   | |
| 2 |00:06    | 3.982  |   V   | day 1 hours: 18.9 |
| 2 |00:07    | 3.982  |   V   | |
| 2 |00:08    | 3.982  |   V   | |
| 2 |02:28    | 3.958  |   V   | |
| 2 |04:59    | 3.944  |   V   | |
| 2 |12:57    | 3.888  |   V   | |
| 2 |13:08    | 3.886  |   V   | |
| 2 |16:44    | 3.862  |   V   | |
| 2 |17:25    | 3.854  |   V   | |
| 2 |18:22    | 3.852  |   V   | day 2 hours: 42.9 |
| 3 |14:54    | 3.768  |   V   | some hours ago the curve got flat |
| 3 |17:33    | 3.764  |   V   | |
| 3 |20:39    | 3.756  |   V   | |
| 3 |23:03    | 3.752  |   V   | day 3 hours: 66.9 |
| 4 |09:22    | 3.694  |   V   | some hours ago the curve got steeper again |
| 4 |14:39    | 3.636  |   V   | |
| 4 |22:00    | 3.584  |   V   | day 4 hours: 90.9 |
| 5 |07:16    | 3.510  |   V   | |
| 5 |09:53    | 3.484  |   V   | |
| 5 |13:26    | 3.450  |   V   | |
| 5 |17:16    | 3.310  |   V   | |
| 5 |17:37    | 3.246  |   V   | random unplanned reboot ;) power too weak? |
| 5 |17:39    | 3.232  |   V   | really plummets now ;( |
| 5 |18:18    | 3.158  |   V   | hours till death: 109.2 |

some time later watch just stopped. 
after reset and just about 1 minute on power it run again

|day| time    | value  | units | remarks |
|---| :-------| -----: | :---: | --------|
| 6 |10:42    | 3.086  |   V   | |
| 6 |10:46    | 3.038  |   V   | |
| 6 |10:48    | 3.006  |   V   | |
| 6 |10:49    | 2.970  |   V   | |
| 6 |10:51    | 2.944  |   V   | |
| 6 |10:55    |        |       | dead again ;( |

recharged it quickly and readings just plummets in second 
from 2.850 to 2.650 and then watch stopped 

# Findings so far

|         voltage           |   V   |  *remarks*   |
|:--------------------------|------:|--------------|
| minimal voltage power on  | 4.194 |              |
| maximal voltage power off | 4.180 |              |
| threshold power on/off    | 4.187 |              |
| discharge voltage range   | 1.380 | 4.18 - 2.8 (battery Voltage at end of Discharge+0.05V) |

| hours | mV drop | mV/h avg|
|------:|--------:|--------:|
|  18.6 |    194  |   10.5  |
|  21.4 |    218  |   10.2  |
|  31.8 |    292  |    9.2  |
|  36.3 |    326  |    9.0  |
|  57.9 |    412  |    7.1  |
|  60.4 |    426  |    7.1  |
|  63.4 |    424  |    6.7  |
|  65.9 |    428  |    6.5  |
|  76.2 |    486  |    6.4  |
|  81.5 |    544  |    6.7  |
|  88.9 |    596  |    6.7  |
|  98.0 |    670  |    6.8  |
| 104.3 |    730  |    7.0  |
| 108.2 |    870  |    8.0  |


* total time from full to 2.8V  : 1.38V / 0.008V/h = **171**h -> **7** days, **3**h, **37**min
* avg current: 180mAh / 194h = **0.9**mA
* probable max current at 1/60 work/sleep ratio: 1.2mA * 60 = **72**mA

# Voltage/TTL function

Voltage/TTL function is an approximation of empirically collected data during discharge cycle

| hours|voltage| rngU | rngT|   coeffUT     |  TTL  | Battery|
|-----:|------:|-----:|----:|--------------:|------:|-------:|
|  0.00| 4.180 |      |     |               |109.57 |100.00% |
| 37.30| 3.850 |-0.330|37.30|-0.008847184987|	72.27 |	65.96% |
| 76.30| 3.700 |-0.150|39.00|-0.003846153846|	33.27 |	30.36% |
|104.37| 3.450 |-0.250|28.07|-0.008906305664|	 5.20 |	 4.75% |
|108.20| 3.310 |-0.140| 3.83|-0.036553524800|	 1.37 |	 1.25% |
|109.23| 3.158 |-0.152| 1.03|-0.147572815500|	 0.34 |	 0.31% |
|109.52| 3.000 |-0.158| 0.29|-0.544827586200|	 0.05 |	 0.05% |
|109.57| 2.850 |-0.150| 0.05|-3.000000000000|	 0.00	|  0.00% |

**TTL<sub>max</sub>**: expected maximum TTL: 109.57h

**u**: measured battery voltage in Volt

**tab(u)**: function to get table entry tab[i] where tab[i].voltage > u > tab[i-1].voltage

**ttl(u)**: TTL at the time of measurement

ttl(u) = TTL<sub>max</sub> - t<sub>elapsed</sub>

t<sub>elapsed</sub> = tab(u).hours + $\Delta t$

$\Delta t$ = $\Delta u$ / tab(u).coeffUT

$\Delta u$ = u - tab(u).voltage

ttl(u) = TTL<sub>max</sub> - (tab(u).hours + ((u - tab(u).voltage) / tab(u).coeffUT))

battery state is just ttl(u) / TTL<sub>max</sub> ratio

example:

ttl(3.8v) = 109.57h - (76.30h + ((3.8V - 3.7V) / -0.003846153846V/h)) 

= 109.57h - (76.30h + (-26,000000001h))

= 109.57h - 50,299999999h

= 59,270000001h ~ 2 days, 11 hours, 16 minutes

battery(3.8v) = 59,270000001h / 109.57h = 0,540932737073 ~ 54%



|day| time    | value  | units | remarks |
|---| :-------| -----: | :---: | --------|
| 1 |16:18    | 3.486  |   V   | |
| 1 |16:18    | 3.512  |   V   | |
| 1 |16:18    | 3.526  |   V   | |
| 1 |16:19    | 3.536  |   V   | |
| 1 |16:21    | 3.576  |   V   | |
| 1 |16:27    | 3.618  |   V   | |
| 1 |16:31    | 3.646  |   V   | |
| 1 |16:38    | 3.694  |   V   | |
| 1 |16:40    | 3.716  |   V   | |
| 1 |16:44    | 3.748  |   V   | |
| 1 |16:54    | 3.810  |   V   | |
| 1 |16:58    | 3.848  |   V   | |
| 1 |17:00    | 3.852  |   V   | |
| 1 |17:03    | 3.856  |   V   | |
| 1 |17:06    | 3.872  |   V   | |
| 1 |17:30    | 3.928  |   V   | |
| 1 |17:33    | 3.940  |   V   | |
| 1 |17:37    | 3.956  |   V   | |
| 1 |17:43    | 3.986  |   V   | |
| 1 |17:    | 3.986  |   V   | |


voltage -> time					
u	du	dt	t	TTL	done
3.824	0.000	0	0	1.91	0.00%
3.850	0.000	0	0.07	1.84	3.67%
3.880	0.000	0	0.24	1.67	12.59%
3.910	0.000	0	0.51	1.40	26.76%
4.178	0.000	0	1.41	0.50	73.97%
4.194	0.000	0	1.52	0.39	79.74%
4.210	0.000	0	1.77	0.14	92.86%
4.214	0.000	0	1.89	0.02	99.15%
