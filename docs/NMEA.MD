# The NMEA 0183 Protocol
# Table of Contents
* [1. What is the NMEA 0183 Standard?](#nmea) 
* [2. Electrical Interface](#electrical)
* [3. General Sentence Format](#sentence)
* [4. Talker Identifiers](#talker-identifier)
* [5. Sentence Identifiers and Sentence Formats](#formats)
* [6. Some Proprietary Sentences](#proprietary-sentences)
* [7. Manufacturer Codes](#codes)
* [8. References](#references)

The material presented in this document has been compiled from various inofficial sources. It is neither a
complete nor error-free description of the NMEA 0183 standard. In particular, it does not cover the new
sentences and the high-speed interface defined in version 3.x.

Klaus Betke, May 2000. Revised August 2001.


## 1. What is the NMEA 0183 Standard? <a id="nmea"></a>
The National Marine Electronics Association (NMEA) is a non-profit association of manufacturers,
distributors, dealers, educational institutions, and others interested in peripheral marine electronics
occupations. The NMEA 0183 standard defines an electrical interface and data protocol for
communications between marine instrumentation.

NMEA 0183 is a voluntary industry standard, first released in March of 1983. It has been updated from
time to time; the latest release, currently (August 1. Version 3.0, July 2001, is available from the
NMEA office (Warning: the price for non-members is 250 US$).

    P O Box 3435
    New Bern NC 28564-3435
    USA
    [www.nmea.org](www.nmea.org "visit nmea")

NMEA has also established a working group to develop a new standard for data communications among
shipboard electronic devices. The new standard, NMEA 2000, is a bi-directional, multi-transmitter,
multi-receiver serial data network. It is multi-master and self-configuring, and there is no central controller.
The NMEA began a beta testing period in January 2000 with eleven manufacturers. A release version of
NMEA 2000 is expected in 2001.

## 2. Electrical Interface  <a id="electrical"></a>
NMEA 0183 devices are designated as either talkers or listeners (with some devices being both),
employing an asynchronous serial interface with the following parameters:
  
**Baud rate**: 4800  
**Number of data bits**: 8 (bit 7 is 1.  
**Stop bits**: 1 (or more)  
**Parity**: none  
**Handshake**: none  

NMEA 0183 allows a single talker and several listeners on one circuit. The recommended interconnect
wiring is a shielded twisted pair, with the shield grounded only at the talker. The standard dos not specify
the use of a particular connector. Note: The new 0183-HS standard (HS = high speed) introduced in
version 3.0 uses a 3-wire interface and a baud rate of 38400. This type of interface is not discussed here.

Its is recommended that the talker output comply with EIA RS-422, a differential system with two signal
lines, "A" and "B". Differential drive signals have no reference to ground and are more immune to noise.
However, a single-ended line at TTL level is accepted as well. The voltages on the A line correspond to
those on the TTL single wire, while the B voltages are inverted (when output A is at +5 V, output B is at
0 V, and vice versa. This is the unipolar RS-422 operation. In bipolar mode ±5 V are used).

In either case, the recommended receive circuit uses an opto-isolator with suitable protection circuitry.
The input should be isolated from the receiver's ground. In practice, the single wire, or the RS-422 "A"
wire may be directly connected to a computer's RS-232 input. In fact even many of the latest products,
like hand-held GPS receivers, do not have a RS-422 differential output, but just a single line with TTL or
5 V CMOS compatible signal level.

## 3. General Sentence Format <a id="sentence"></a>

All data is transmitted in the form of sentences. Only printable ASCII characters are allowed, plus CR
(carriage return) and LF (line feed). Each sentence starts with a "$" sign and ends with <CR><LF>. There
are three basic kinds of sentences: talker sentences, proprietary sentences and query sentences.

**Talker Sentences.** The general format for a talker sentence is:
```
$ttsss,d1,d2,....<CR><LF>
```
The first two letters following the „$” are the talker identifier. The next three characters (sss) are the
sentence identifier, followed by a number of data fields separated by commas, followed by an optional
checksum, and terminated by carriage return/line feed. The data fields are uniquely defined for each
sentence type. An example talker sentence is:

```$HCHDM,238,M<CR><LF>```

where "HC" specifies the talker as being a magnetic compass, the "HDM" specifies the magnetic heading
message follows. The "238" is the heading value, and "M" designates the heading value as magnetic.

A sentence may contain up to 80 characters plus "$" and CR/LF. If data for a field is not available, the
field is omitted, but the delimiting commas are still sent, with no space between them. The checksum
field consists of a "*" and two hex digits representing the exclusive OR of all characters between, but not
including, the "$" and "*".

**Proprietary Sentences**. The standard allows individual manufacturers to define proprietary sentence
formats. These sentences start with "$P", then a 3 letter manufacturer ID, followed by whatever data the
manufacturer wishes, following the general format of the standard sentences. Some proprietary
sentences, mainly from Garmin, Inc., are listed in chapter 6.

**Query sentences.** A query sentence is a means for a listener to request a particular sentence from a
talker. The general format is:

    $ttllQ,sss,[CR][LF]

The first two characters of the address field are the talker identifier of the requester and the next two
characters are the talker identifier of the device being queried (listener). The fifth character is always a "Q"
defining the message as a query. The next field (sss) contains the three letter mnemonic of the sentence
being requested. An example query sentence is:

    $CCGPQ,GGA<CR><LF>

where the "CC" device (computer) is requesting from the "GP" device (a GPS unit) the "GGA" sentence.
The GPS will then transmit this sentence once per second until a different query is requested.

## 4. Talker Identifiers <a id="talker-identifier"></a>
    AG Autopilot - General  
    AP Autopilot - Magnetic
    CD Communications – Digital Selective Calling (DSC)
    CR Communications – Receiver / Beacon Receiver
    CS Communications – Satellite
    CT Communications – Radio-Telephone (MF/HF)
    CV Communications – Radio-Telephone (VHF)
    CX Communications – Scanning Receiver
    DF Direction Finder
    EC Electronic Chart Display & Information System (ECDIS)
    EP Emergency Position Indicating Beacon (EPIRB)
    ER Engine Room Monitoring Systems
    GP Global Positioning System (GPS)
    HC Heading – Magnetic Compass
    HE Heading – North Seeking Gyro
    HN Heading – Non North Seeking Gyro
    II Integrated Instrumentation
    IN Integrated Navigation
    LC Loran C
    P Proprietary Code
    RA RADAR and/or ARPA
    SD Sounder, Depth
    SN Electronic Positioning System, other/general
    SS Sounder, Scanning
    TI Turn Rate Indicator
    VD Velocity Sensor, Doppler, other/general
    DM Velocity Sensor, Speed Log, Water, Magnetic
    VW Velocity Sensor, Speed Log, Water, Mechanical
    WI Weather Instruments
    YX Transducer
    ZA Timekeeper – Atomic Clock
    ZC Timekeeper – Chronometer
    ZQ Timekeeper – Quartz
    ZV Timekeeper – Radio Update, WWV or WWVH
    
## 5. Sentence Identifiers and Formats <a id="formats"></a>
**AAM
Waypoint Arrival Alarm**
```
       1 2 3   4 5    6
       | | |   | |    |
$--AAM,A,A,x.x,N,c--c*hh
```
1. Status, BOOLEAN, A = Arrival circle entered
1. Status, BOOLEAN, A = perpendicular passed at waypoint
1. Arrival circle radius
1. Units of radius, nautical miles
1. Waypoint ID
1. Checksum

**ALM GPS Almanac Data**
```
       1   2   3  4   5  6    7  8    9    10     11    12      13     14  15   16
       |   |   |  |   |  |    |  |    |    |      |      |      |      |   |    |
$--ALM,x.x,x.x,xx,x.x,hh,hhhh,hh,hhhh,hhhh,hhhhhh,hhhhhh,hhhhhh,hhhhhh,hhh,hhh,*hh
```

1. APA
1. Total number of messages
1. Message Number
1. Satellite PRN number (01 to 1.
1. GPS Week Number: Date and time in GPS is computed as number of weeks
1. from 6 January 1980 plus number of seconds into the week.
1. SV health, bits 17-24 of each almanac page
1. Eccentricity
1. Almanac Reference Time
1. Inclination Angle
1. Rate of Right Ascension
1. Root of semi-major axis
1. Argument of perigee
1. Longitude of ascension node
1. Mean anomaly
1. F0 Clock Parameter
1. F1 Clock Parameter
1. Checksum

**APA Autopilot Sentence "A"**
```
       1 2  3   4 5 6 7  8  9 10    11
       | |  |   | | | |  |  | |     |
$--APA,A,A,x.xx,L,N,A,A,xxx,M,c---c*hh
```
1. Status  
    V = LORAN-C Blink or SNR warning  
    A = general warning flag or other navigation systems when a reliable fix is not available  
1. Status  
    V = Loran-C Cycle Lock warning flag  
    A = OK or not used
1. Cross Track Error Magnitude
1. Direction to steer, L or R
1. Cross Track Units (Nautic miles or kilometres)
1. Status
A = Arrival Circle Entered
1. Status
A = Perpendicular passed at waypoint
1. Bearing origin to destination
1. M = Magnetic, T = True
1. Destination Waypoint ID
1. checksum


**APB Autopilot Sentence "B"**  
```
                                        13    15
       1 2 3   4 5 6 7 8   9 10   11 12 |   14|
       | | |   | | | | |   | |    |   | |   | |
$--APB,A,A,x.x,a,N,A,A,x.x,a,c--c,x.x,a,x.x,a*hh
```
1. Status
V = LORAN-C Blink or SNR warning
A = general warning flag or other navigation systems when a reliable
fix is not available
1. Status
V = Loran-C Cycle Lock warning flag
A = OK or not used
1. Cross Track Error Magnitude
1. Direction to steer, L or R
1. Cross Track Units, N = Nautical Miles
1. Status
A = Arrival Circle Entered
1. Status
A = Perpendicular passed at waypoint
1. Bearing origin to destination
1. M = Magnetic, T = True
1. Destination Waypoint ID
1. Bearing, present position to Destination
1. M = Magnetic, T = True
1. Heading to steer to destination waypoint
1. M = Magnetic, T = True
1. Checksum

**ASD Autopilot System Data**
Format unknown

**BEC Bearing & Distance to Waypoint – Dead Reckoning**
```
                                                        12
       1         2       3 4        5 6   7 8   9 10  11|    13
       |         |       | |        | |   | |   | |   | |    |
$--BEC,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x.x,T,x.x,M,x.x,N,c--c*hh
```
1. Time (UTC)
1. Waypoint Latitude
1. N = North, S = South
1. Waypoint Longitude
1. E = East, W = West
1. Bearing, True
1. T = True
1. Bearing, Magnetic
1. M = Magnetic
1. Nautical Miles
1. N = Nautical Miles
1. Waypoint ID
1. Checksum

**BOD Bearing – Waypoint to Waypoint**
```

       1   2 3   4 5    6    7
       |   | |   | |    |    |
$--BOD,x.x,T,x.x,M,c--c,c--c*hh
```
1. Bearing Degrees, TRUE
1. T = True
1. Bearing Degrees, Magnetic
1. M = Magnetic
1. TO Waypoint
1. FROM Waypoint
1. Checksum

**BWC Bearing and Distance to Waypoint – Latitude, N/S, Longitude, E/W, UTC, Status**
```                 
                                                      11
       1         2       3 4        5 6   7 8   9 10  | 12   13
       |         |       | |        | |   | |   | |   | |    |
$--BWC,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x.x,T,x.x,M,x.x,N,c--c*hh
```
1. Time (UTC)
1. Waypoint Latitude
1. N = North, S = South
1. Waypoint Longitude
1. E = East, W = West
1. Bearing, True
1. T = True
1. Bearing, Magnetic
1. M = Magnetic
1. Nautical Miles
1. N = Nautical Miles
1. Waypoint ID
1. Checksum


**BWR Bearing and Distance to Waypoint – Rhumb Line Latitude, N/S, Longitude, E/W,UTC, Status**
```  
                                                      11
       1         2       3 4        5 6   7 8   9 10  | 12   13
       |         |       | |        | |   | |   | |   | |    |
$--BWR,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x.x,T,x.x,M,x.x,N,c--c*hh

```
1. Time (UTC)
1. Waypoint Latitude
1. N = North, S = South
1. Waypoint Longitude
1. E = East, W = West
1. Bearing, True
1. T = True
1. Bearing, Magnetic
1. M = Magnetic
1. Nautical Miles
1. N = Nautical Miles
1. Waypoint ID
1. Checksum

**BWW Bearing – Waypoint to Waypoint**
```

       1   2 3   4 5    6    7
       |   | |   | |    |    | 
$--BWW,x.x,T,x.x,M,c--c,c--c*hh
```
1. Bearing Degrees, TRUE
1. T = True
1. Bearing Degrees, Magnetic
1. M = Magnetic
1. TO Waypoint
1. FROM Waypoint
1. Checksum

**DBK Depth Below Keel**
```
       1   2 3   4 5   6 7
       |   | |   | |   | |
$--DBK,x.x,f,x.x,M,x.x,F*hh
```
1. Depth, feet
1. f = feet
1. Depth, meters
1. M = meters
1. Depth, Fathoms
1. F = Fathoms
1. Checksum

**DBS Depth Below Surface**
```
       1   2 3   4 5   6 7
       |   | |   | |   | |
$--DBS,x.x,f,x.x,M,x.x,F*hh
```
1. Depth, feet
1. f = feet
1. Depth, meters
1. M = meters
1. Depth, Fathoms
1. F = Fathoms
1. Checksum

**DBT Depth Below Transducer**
```
       1   2 3   4 5   6 7
       |   | |   | |   | |
$--DBT,x.x,f,x.x,M,x.x,F*hh
```
1. Depth, feet
1. f = feet
1. Depth, meters
1. M = meters
1. Depth, Fathoms
1. F = Fathoms
1. Checksum

**DCN Decca Position**
obsolete

**DPT Heading – Deviation & Variation**
```
       1   2   3
       |   |   |
$--DPT,x.x,x.x*hh
``
1. Depth, meters
1. Offset from transducer;
positive means distance from transducer to water line,
negative means distance from transducer to keel
1. Checksum

**DSC Digital Selective Calling Information**
Format unknown

**DSE Extended DSC**
Format unknown

**DSI DSC Transponder Initiate**
Format unknown

**DSR DSC Transponder Response**
Format unknown

**DTM Datum Reference**
Format unknown

**FSI Frequency Set Information**
```
       1      2      3 4 5
       |      |      | | |
$--FSI,xxxxxx,xxxxxx,c,x*hh
```
1. Transmitting Frequency
1. Receiving Frequency
1. Communications Mode (NMEA Syntax 1.
1. Power Level
1. Checksum

**GBS GPS Satellite Fault Detection*
Format unknown

**GGA Global Positioning System Fix Data. Time, Position and fix related data for a GPS receiver**
```
                                                     11    13
       1         2       3 4        5 6 7  8   9   10|   12|   14   15
       |         |       | |        | | |  |   |   | |   | |   |    |
$--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
```
1. GLC
1. Time (UTC)
1. Latitude
1. N or S (North or South)
1. Longitude
1. E or W (East or West)
1. GPS Quality Indicator,
0 - fix not available,
1 - GPS fix,
2 - Differential GPS fix
1. Number of satellites in view, 00 - 12
1. Horizontal Dilution of precision
1. Antenna Altitude above/below mean-sea-level (geoid)
1. Units of antenna altitude, meters
1. Geoidal separation, the difference between the WGS-84 earth
1. ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid
1. Units of geoidal separation, meters
1. Age of differential GPS data, time in seconds since last SC104
type 1 or 9 update, null field when DGPS is not used
1. Differential reference station ID, 0000-1023
1. Checksum

**GLC Geographic Position, Loran-C**
```
                                          12    14
       1    2   3 4   5 6   7   8 9 10  11|   13|
       |    |   | |   | |   |   | | |   | |   | |
$--GLC,xxxx,x.x,a,x.x,a,x.x,a.x,x,a,x.x,a,x.x,a*hh
```
1. GLL
1. GRI Microseconds/10
1. Master TOA Microseconds
1. Master TOA Signal Status
1. Time Difference 1 Microseconds
1. Time Difference 1 Signal Status
1. Time Difference 2 Microseconds
1. Time Difference 2 Signal Status
1. Time Difference 3 Microseconds
1. Time Difference 3 Signal Status
1. Time Difference 4 Microseconds
1. Time Difference 4 Signal Status
1. Time Difference 5 Microseconds
1. Time Difference 5 Signal Status
1. Checksum

** GLL Geographic Position – Latitude/Longitude**
```
       1       2 3        4 5         6 7
       |       | |        | |         | |
$--GLL,llll.ll,a,yyyyy.yy,a,hhmmss.ss,A*hh
```
1. Latitude
1. N or S (North or South)
1. Longitude
1. E or W (East or West)
1. Time (UTC)
1. Status A - Data Valid, V - Data Invalid
1. Checksum


**GRS GPS Range Residuals**
Format unknown

**GST GPS Pseudorange Noise Statistics*
Format unknown

**GSA GPS DOP and active satellites**
```

       1 2 3                         14 15  16  17 18
       | | |                         |  |   |   |  |
$--GSA,a,a,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x.x,x.x,x.x*hh
```
1. Selection mode
1. Mode
1. ID of 1st satellite used for fix
1. ID of 2nd satellite used for fix
...
14. ID of 12th satellite used for fix
15. PDOP in meters
16. HDOP in meters
17. VDOP in meters
18. Checksum

**GSV Satellites in view**
```
       1 2 3 4 5 6 7     n
       | | | | | | |     |
$--GSV,x,x,x,x,x,x,x,...*hh
```
1. total number of messages
1. message number
1. satellites in view
1. satellite number
1. elevation in degrees
1. azimuth in degrees to true
1. SNR in dB
more satellite infos like 1.-1.
n. Checksum

**GTD Geographic Location in Time Differences**
```
       1   2   3   4   5   6
       |   |   |   |   |   |
$--GTD,x.x,x.x,x.x,x.x,x.x*hh
```
1. time difference
1. time difference
1. time difference
1. time difference
1. time difference
1. Checksum

**GXA TRANSIT Position – Latitude/Longitude, Location and Time of TRANSIT Fix at Waypoint**
obsolete

**HDG Heading – Deviation & Variation**
```
       1   2   3 4   5 6
       |   |   | |   | |
$--HDG,x.x,x.x,a,x.x,a*hh
```
1. Magnetic sensor heading in degrees
1. Magnetic sensor Deviation, degrees
1. Magnetic sensor Deviation direction, E = Easterly, W = Westerly
1. Magnetic sensor Variation degrees
1. Magnetic sensor Variation direction, E = Easterly, W = Westerly
1. Checksum


**HDM Heading -Magnetic**
```
       1   2 3
       |   | |
$--HDM,x.x,M*hh
```
1. Heading Degrees, magnetic
1. M = magnetic
1. Checksum

**HDT Heading – True**
```
       1   2 3
       |   | |
$--HDT,x.x,T*hh
```
1. Heading Degrees, true
1. T = True
1. Checksum

**HSC Heading Steering Command**
```
       1   2 3   4  5
       |   | |   |  |
$--HSC,x.x,T,x.x,M,*hh
```
1. Heading Degrees, True
1. T = True
1. Heading Degrees, Magnetic
1. M = Magnetic
1. Checksum

**LCD Loran-C Signal Data**
```
       1    2   3   4   5   6   7   8   9   10  11  12  13  14
       |    |   |   |   |   |   |   |   |   |   |   |   |   |
$--LCD,xxxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx*hh
```
1. GRI Microseconds/10
1. Master Relative SNR
1. Master Relative ECD
1. Time Difference 1 Microseconds
1. Time Difference 1 Signal Status
1. Time Difference 2 Microseconds
1. Time Difference 2 Signal Status
1. Time Difference 3 Microseconds
1. Time Difference 3 Signal Status
1. Time Difference 4 Microseconds
1. Time Difference 4 Signal Status
1. Time Difference 5 Microseconds
1. Time Difference 5 Signal Status
1. Checksum

**MSK MSK Receiver Interface (for DGPS Beacon Receivers)**
```
       1     2  3   4  5 6
       |     |  |   |  | |
$GPMSK,xxx.x,xx,xxx,xx,N*hh
```
1. Frequency in kHz (283.5 to 325.1.
1. Frequency Selection
M1 = Manual
A1 = Automatic (field 1 empty)
1. MSK bit rate (100 or 1.
1. Bit Rate Selection
M2 = Manual
A2 = Automatic (field 3 empty)
1. Period of output of performance status message, 0 to 100 seconds ($CRMSS)
1. Checksum

**MSS MSK Receiver Signal Status**
Format unknown

**MWD Wind Direction & Speed**
Format unknown

**MTW Water Temperature**
```
       1   2 3
       |   | |
$--MTW,x.x,C*hh
```
1. Degrees
1. Unit of Measurement, Celcius
1. Checksum

**MWV Wind Speed and Angle**
```
       1   2 3   4 5
       |   | |   | |
$--MWV,x.x,a,x.x,a*hh
```
1. Wind Angle, 0 to 360 degrees
1. Reference, R = Relative, T = True
1. Wind Speed
1. Wind Speed Units, K/M/N
1. Status, A = Data Valid
1. Checksum

**OLN Omega Lane Numbers**
obsolete

**OSD Own Ship Data**
```
       1   2 3   4 5   6 7   8   9 10
       |   | |   | |   | |   |   | |
$--OSD,x.x,A,x.x,a,x.x,a,x.x,x.x,a*hh
```
1. Heading, degrees true
1. Status, A = Data Valid
1. Vessel Course, degrees True
1. Course Reference
1. Vessel Speed
1. Speed Reference
1. Vessel Set, degrees True
1. Vessel drift (speed)
1. Speed Units
1. Checksum

**ROO Waypoints in Active Route**
```
$--ROO,c---c,c---c,....*hh
```
1. waypoint ID
...
n) checksum
RMA
Recommended Minimum Navigation Information
12
1 2
3 4
5 6
7
8
9
10 11|
| |
| |
| |
|
|
|
|
| |
$--RMA,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,x.x,x.x,a*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
Blink Warning
Latitude
N or S
Longitude
E or W
Time Difference A, μS
Time Difference B, μS
Speed Over Ground, Knots
Track Made Good, degrees true
Magnetic Variation, degrees
E or W
Checksum
13The NMEA 0183 Protocol
RMB
Recommended Minimum Navigation Information
14
1 2
3 4
5
6
7 8
9 10 11 12 13|
| |
| |
|
|
| |
| |
|
|
| |
$--RMB,A,x.x,a,c--c,c--c,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,A*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
Status, V = Navigation receiver warning
Cross Track error - nautical miles
Direction to Steer, Left or Right
TO Waypoint ID
FROM Waypoint ID
Destination Waypoint Latitude
N or S
Destination Waypoint Longitude
E or W
Range to destination in nautical miles
Bearing to destination in degrees True
Destination closing velocity in knots
Arrival Status, A = Arrival Circle Entered
Checksum
RMC
Recommended Minimum Navigation Information
12
1
2 3
4 5
6 7
8
9
10 11|
|
| |
| |
| |
|
|
|
| |
$--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
Time (UTC)
Status, V = Navigation receiver warning
Latitude
N or S
Longitude
E or W
Speed over ground, knots
Track made good, degrees true
Date, ddmmyy
Magnetic Variation, degrees
E or W
Checksum
ROT
Rate Of Turn
1
2 3
|
| |
$--ROT,x.x,A*hh
1. Rate Of Turn, degrees per minute, "-" means bow turns to port
1. Status, A means data is valid
1. Checksum
RPM
Revolutions
1 2 3
4
5 6
| | |
|
| |
$--RPM,a,x,x.x,x.x,A*hh
1.
1.
1.
1.
1.
1.
Source; S = Shaft, E = Engine
Engine or shaft number
Speed, Revolutions per minute
Propeller pitch, % of maximum, "-" means astern
Status, A means data is valid
Checksum
14The NMEA 0183 Protocol
RSA
Rudder Sensor Angle
1
2 3
4 5
|
| |
| |
$--RSA,x.x,A,x.x,A*hh
1.
1.
1.
1.
1.
Starboard (or single) rudder sensor, "-" means Turn To Port
Status, A means data is valid
Port rudder sensor
Status, A means data is valid
Checksum
RSD
RADAR System Data
14
1
2
3
4
5
6
7
8
9
10 11 12 13|
|
|
|
|
|
|
|
|
|
|
|
| | |
$--RSD,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,a,a*hh
1.
1.
1.
1.
1.
RTE
Cursor Range From Own Ship
Cursor Bearing Degrees Clockwise From Zero
Range Scale
Range Units
Checksum
Routes
1
2
3 4
5
x
n
|
|
| |
|
|
|
$--RTE,x.x,x.x,a,c--c,c--c, ..... c--c*hh
1. Total number of messages being transmitted
1. Message Number
1. Message mode
c = complete route, all waypoints
w = working route, the waypoint you just left, the waypoint you're heading to,
then all the rest
1. Waypoint ID
x) More Waypoints
n) Checksum
SFI
Scanning Frequency Information
1
2
3
4
n
|
|
|
|
|
$--SFI,x.x,x.x,xxxxxx,c .......... xxxxxx,c*hh
1.
1.
1.
1.
n)
Total Number Of Messages
Message Number
Frequency 1
Mode 1
Checksum
STN
Multiple Data ID
1
2
|
|
$--STN,x.x,*hh
1. Talker ID Number
1. Checksum
15The NMEA 0183 Protocol
TLL
Target Latitude and Longitude
Format unknown
TRF
TRANSIT Fix Data
obsolete
TTM
Tracked Target Message
11
13
1 2
3
4 5
6
7 8
9
10|
12| 14
| |
|
| |
|
| |
|
| |
| | |
$--TTM,xx,x.x,x.x,a,x.x,x.x,a,x.x,x.x,a,c--c,a,a*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
Target Number
Target Distance
Bearing from own ship
Bearing Units
Target speed
Target Course
Course Units
Distance of closest-point-of-approach
Time until closest-point-of-approach "-" means increasing
"-" means increasing
Target name
Target Status
Reference Target
Checksum
VBW
Dual Ground/Water Speed
1
2
3 4
5
6 7
|
|
| |
|
| |
$--VBW,x.x,x.x,A,x.x,x.x,A*hh
1.
1.
1.
1.
1.
1.
1.
Longitudinal water speed, "-" means astern
Transverse water speed, "-" means port
Status, A = data valid
Longitudinal ground speed, "-" means astern
Transverse ground speed, "-" means port
Status, A = data valid
Checksum
VDR
Set and Drift
1
2 3
4 5
6 7
|
| |
| |
| |
$--VDR,x.x,T,x.x,M,x.x,N*hh
1.
1.
1.
1.
1.
1.
1.
Degress True
T = True
Degrees Magnetic
M = Magnetic
Knots (speed of current)
N = Knots
Checksum
16The NMEA 0183 Protocol
VHW
Water Speed and Heading
1
2 3
4 5
6 7
8 9
|
| |
| |
| |
| |
$--VHW,x.x,T,x.x,M,x.x,N,x.x,K*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
Degress True
T = True
Degrees Magnetic
M = Magnetic
Knots (speed of vessel relative to the water)
N = Knots
Kilometers (speed of vessel relative to the water)
K = Kilometres
Checksum
VLW
Distance Traveled through Water
1
2 3
4 5
|
| |
| |
$--VLW,x.x,N,x.x,N*hh
1.
1.
1.
1.
1.
Total cumulative distance
N = Nautical Miles
Distance since Reset
N = Nautical Miles
Checksum
VPW
Speed – Measured Parallel to Wind
1
2 3
4 5
|
| |
| |
$--VPW,x.x,N,x.x,M*hh
1.
1.
1.
1.
1.
Speed, "-" means downwind
N = Knots
Speed, "-" means downwind
M = Meters per second
Checksum
VTG
Track Made Good and Ground Speed
1
2 3
4 5 6 7
8 9
|
| |
| | | |
| |
$--VTG,x.x,T,x.x,M,x.x,N,x.x,K*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
Track Degrees
T = True
Track Degrees
M = Magnetic
Speed Knots
N = Knots
Speed Kilometers Per Hour
K = Kilometres Per Hour
Checksum
17The NMEA 0183 Protocol
VWR
18
Relative Wind Speed and Angle
1 2 3 4 5 6 7 8 9
| | | | | | | | |
$--VWR,x.x,a,x.x,N,x.x,M,x.x,K*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
Wind direction
Wind direction
Speed
N = Knots
Speed
M = Meters Per
Speed
K = Kilometers
Checksum
WCV
magnitude in degrees
Left/Right of bow
Second
Per Hour
Waypoint Closure Velocity
1
2 3
4
|
| |
|
$--WCV,x.x,N,c--c*hh
1.
1.
1.
1.
Velocity
N = knots
Waypoint ID
Checksum
WDC
Distance to Waypoint – Great Circle
Format unknown
WDR
Distance to Waypoint – Rhumb Line
Format unknown
WNC
Distance – Waypoint to Waypoint
1
2 3
4 5
6
7
|
| |
| |
|
|
$--WNC,x.x,N,x.x,K,c--c,c--c*hh
1.
1.
1.
1.
1.
1.
1.
Distance, Nautical Miles
N = Nautical Miles
Distance, Kilometers
K = Kilometers
TO Waypoint
FROM Waypoint
Checksum
WPL
Waypoint Location
1
2 3
4 5
6
|
| |
| |
|
$--WPL,llll.ll,a,yyyyy.yy,a,c--c*hh
1.
1.
1.
1.
1.
1.
Latitude
N or S (North or South)
Longitude
E or W (East or West)
Waypoint Name
ChecksumThe NMEA 0183 Protocol
XDR
Cross Track Error – Dead Reckoning
1 2
3 4
n
| |
| |
|
$--XDR,a,x.x,a,c--c, ..... *hh
1.
1.
1.
1.
x)
n)
Transducer type
Measurement data
Units of measurement
Name of transducer
More of the same
Checksum
XTE
Cross-Track Error – Measured
1 2 3
4 5 6
| | |
| | |
$--XTE,A,A,x.x,a,N,*hh
1. Status
V = LORAN-C blink or SNR warning
A = general warning flag or other navigation systems when a reliable
fix is not available
1. Status
V = Loran-C cycle lock warning flag
A = OK or not used
1. Cross track error magnitude
1. Direction to steer, L or R
1. Cross track units. N = Nautical Miles
1. Checksum
XTR
Cross Track Error – Dead Reckoning
1
2 3 4
|
| | |
$--XTR,x.x,a,N*hh
1.
1.
1.
1.
Magnitude of cross track error
Direction to steer, L or R
Units, N = Nautical Miles
Checksum
ZDA
Time & Date – UTC, Day, Month, Year and Local Time Zone
1
2 3 4
5 6 7
|
| | |
| | |
$--ZDA,hhmmss.ss,xx,xx,xxxx,xx,xx*hh
1.
1.
1.
1.
1.
1.
1.
Local zone minutes description, same sign as local hours
Local zone description, 00 to +/- 13 hours
Year
Month, 01 to 12
Day, 01 to 31
Time (UTC)
Checksum
ZDL
Time and Distance to Variable Point
Format unknown
19The NMEA 0183 Protocol
ZFO
UTC & Time from Origin Waypoint
1
2
3
4
|
|
|
|
$--ZFO,hhmmss.ss,hhmmss.ss,c--c*hh
1.
1.
1.
1.
Time (UTC)
Elapsed Time
Origin Waypoint ID
Checksum
ZTG
UTC & Time to Destination Waypoint
1
2
3
4
|
|
|
|
$--ZTG,hhmmss.ss,hhmmss.ss,c--c*hh
1.
1.
1.
1.
Time (UTC)
Time Remaining
Destination Waypoint ID
Checksum
20The NMEA 0183 Protocol
6. Some Proprietary Sentences
$PGRMC Sensor Configuration Information
Garmin proprietary sentence
13 14
1 2
3 4
5
6
7
8
9 10| 12|
| |
| |
|
|
|
|
| | | | |
$PGRMC,A,x.x,hh,x.x,x.x,x.x,x.x,x.x,c,c,2,c*hh
1. Fix mode, A=automatic (only option)
1. Altitude above/below mean sea level, -1500.0 to 18000.0 meters
1. Earth datum index. If the user datum index (1. is specified,
fields 5-8 must contain valid values. Otherwise, fields 4-8 must be null.
1. User earth datum semi-major axis, 6360000.0 to 6380000.0 meters (.001 meters
resolution)
1. User earth datum inverse flattening factor, 285.0 to 310.0 (10-9 resolution)
1. User earth datum delta x earth centered coordinate, -5000.0 to 5000.0 meters
(1 meter resolution)
1. User earth datum delta y earth centered coordinate, -5000.0 to 5000.0 meters
(1 meter resolution)
1. User earth datum delta z earth centered coordinate, -5000.0 to 5000.0 meters
(1 meter resolution)
1. Differential mode, A = automatic (output DGPS data when available, non-DGPs
otherwise), D = differential exclusively (output only differential fixes)
1. NMEA Baud rate, 1 = 1200, 2 = 2400, 3 = 4800, 4 = 9600
1. Filter mode, 2 = no filtering (only option)
1. PPS mode, 1 = No PPS, 2 = 1 Hz
1. Checksum
$PGRME Estimated Position Error
Garmin proprietary sentence
1
2 3
4 5
6 7
|
| |
| |
| |
$PGRME,x.x,M,x.x,M,x.x,M*hh
1.
1.
1.
1.
1.
1.
1.
Estimated horizontal position error (HPE)
Unit, metres
Estimated vertical error (VPE)
Unit, metres
Overall spherical equivalent position error
Unit, metres
Checksum
21The NMEA 0183 Protocol
$PGRMF Position Fix Sentence
Garmin proprietary sentence
10 12
15
1
2
3
4
5
6
7 8
9 | 11|
13 14| 16
|
|
|
|
|
|
| |
| | | |
|
| | |
$PGRMF,x.x,x.x,ddmmyy,hhmmss,x.x,ddmm.mmmm,c,dddmm.mmmm,c,c,c,x.x,x.x,c,c*hh
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
1.
GPS week number (0 - 1.
GPS seconds (0 - 1.
UTC date of position fix
UTC time of position fix
GPS leap second count
Latitude
N or S
Longitude
E or W
Mode
M = manual
A = automatic
Fix type
0 = no fix
1 = 2D fix
2 = 3D fix
Speed over ground, 0 to 999 kilometers/hour
Course over ground, 0 to 359 degrees, true
Position dilution of precision, 0 to 9 (rounded to nearest integer value)
Time dilution of precision, 0 to 9 (rounded to nearest integer value)
Checksum
$PGRMI Sensor Initialisation Information
Garmin proprietary sentence
1
2 3
4 5
6
7
|
| |
| |
|
|
$PGRMI,ddmm.mmm,N,ddmm.mmm,E,ddmmyy,hhmmss*hh
1.
1.
1.
1.
1.
1.
1.
Latitude
N or S
Longitude
E or W
Current UTC date
Current UTC time
Checksum
$PGRMM Map Datum
Garmin proprietary sentence
1
2
|
|
$PGRMM,c---c*hh
1. Currently active horizontal datum (WGS-84, NAD27 Canada, ED50, a.s.o)
1. Checksum
22The NMEA 0183 Protocol
$PGRMO Output Sentence Enable/Disable
Garmin proprietary sentence
1
2 3
|
| |
$PGRMO,ccccc,c*hh
1. Target sentence description (e.g., PGRMT, GPGSV, etc.)
1. Target sentence mode
0 = disable specified sentence
1 = enable specified sentence
2 = disable all
3 = enable all output sentences (except GPALM)
1. Checksum
$PGRMT Sensor Status Information
Garmin proprietary sentence
1
2 3 4 5 6 7 8
9 10
|
| | | | | | |
| |
$PGRMT,c...c,c,c,c,c,c,c,x.x,c*hh
1. Product, model and software version
e.g. "GPS25VEE] 1.10"
1. Rom checksum test
P = pass
F = fail
1. Receiver failure discrete
P = pass
F = fail
1. Stored data lost
R = retained
L = lost
1. Real time clock lost
R = retained
L = lost
1. 0scillator drift discrete
P = pass
F = excessive drift detected
1. Data collection discrete
C = collecting
null if not collecting
1. Board temperature in degrees C
1. Board configuration data
R = retained
L = lost
1. Checksum
$PGRMV 3D Velocity
Garmin proprietary sentence
1
2
3
4
|
|
|
|
$PGRMV,x.x,x.x,x.x*hh
1.
1.
1.
1.
True east velocity, -999.9 to 9999.9 meters/second
True north velocity, -999.9 to 9999.9 meters/second
Up velocity, -999.9 to 9999.9 meters/second
Checksum
23The NMEA 0183 Protocol
$PGRMZ Altitude Information
Garmin proprietary sentence
1
2 3 4
|
| | |
$PGRMZ,x.x,f,h*hh
1. Altitude
1. Unit, feets
1. Position fix dimensions
2 user altitude
3 GPS altitude
1. Checksum
$PSLIB
Differental GPS Beacon Receiver Control
Starlink, Inc. proprietary sentence, used by Garmin and others
1
2
3 4
|
|
| |
$PSLIB,x.x,x.x,c*hh
1. Frequency
1. Bit rate
1. Request type
J = status request
K = configuration request
blank = tuning message
1. Checksum
24The NMEA 0183 Protocol
7. Manufacturer Codes
Note: This list is out-of-date, but perhaps still useful.
AAR
ACE
ACR
ACS
ACT
AGI
AHA
AIP
ALD
AMR
AMT
ANS
ANX
ANZ
APC
APN
APX
AQC
AQD
AQM
ASP
ATE
ATM
ATR
ATV
AVN
AWA
BBL
BBR
BDV
BEC
BGS
BGT
BHE
BHR
BLB
BME
BNI
BNS
BRM
BRY
BTH
BTK
BTS
BXA
CAT
CBN
CCA
CCC
CCL
CCM
CDC
CEC
CHI
CKM
CMA
CMC
Asian American Resources
Auto-Comm Engineering Corporation
ACR Electronics, Inc.
Arco Solar, Inc.
Advanced Control Technology
Airguide Instrument Company
Autohelm of America
Aiphone Corporation
Alden Electronics, Inc.
AMR Systems
Airmar Technology
Antenna Specialists
Analytyx Electronic Systems
Anschutz of America
Apelco
American Pioneer, Inc.
Amperex, Inc.
Aqua-Chem, Inc.
Aquadynamics, Inc.
Aqua Meter Instrument Company
American Solar Power
Aetna Engineering
Atlantic Marketing Company, Inc.
Airtron
Activation, Inc.
Advanced Navigation, Inc.
Awa New Zealand, Limited
BBL Industries, Inc.
BBR and Associates
Brisson Development, Inc.
Boat Electric Company
Barringer Geoservice
Brookes and Gatehouse, Inc.
BH Electronics
Bahr Technologies, Inc.
Bay Laboratories
Bartel Marine Electronics
Neil Brown Instrument Systems
Bowditch Navigation Systems
Mel Barr Company
Byrd Industries
Benthos, Inc.
Baltek Corporation
Boat Sentry, Inc.
Bendix-Avalex, Inc.
Catel
Cybernet Marine Products
Copal Corporation of America
Coastal Communications Company
Coastal Climate Company
Coastal Communications
Cordic Company
Ceco Communications, Inc.
Charles Industries, Limited
Cinkel Marine Electronics Industries
Societe Nouvelle D'Equiment du Calvados
Coe Manufacturing Company
25
CME
CMP
CMS
CMV
CNV
CNX
CPL
CPN
CPS
CPT
CRE
CRO
CRY
CSI
CSM
CST
CSV
CTA
CTB
CTC
CTE
CTL
CNI
CWD
CWV
CYZ
DCC
DEB
DFI
DGC
DME
DMI
DNS
DNT
DPS
DRL
DSC
DYN
DYT
EBC
ECT
EEV
EFC
ELD
EMC
EMS
ENA
ENC
EPM
EPT
ERC
ESA
FDN
FHE
FJN
FMM
FNT
FRC
FTG
FUJ
FEC
Cushman Electronics, Inc.
C-Map, s.r.l.
Coastal Marine Sales Company
CourseMaster USA, Inc.
Coastal Navigator
Cynex Manufactoring Company
Computrol, Inc.
Compunav
Columbus Positioning, Inc.
CPT, Inc.
Crystal Electronics, Limited
The Caro Group
Crystek Crystals Corporation
Communication Systems International, Inc.
Comsat Maritime Services
Cast, Inc.
Combined Services
Current Alternatives
Cetec Benmar
Cell-tech Communications
Castle Electronics
C-Tech, Limited
Continental Instruments
Cubic Western Data
Celwave R.F., Inc.
cYz, Inc.
Dolphin Components Corporation
Debeg GmbH
Defender Industries, Inc.
Digicourse, Inc.
Digital Marine Electronics Corp.
Datamarine International, Inc.
Dornier System GmbH
Del Norte Technology, Inc.
Danaplus, Inc.
R.L. Drake Company
Dynascan Corporation
Dynamote Corporation
Dytek Laboratories, Inc.
Emergency Beacon Corporation
Echotec, Inc.
EEV, Inc.
Efcom Communication Systems
Electronic Devices, Inc.
Electric Motion Company
Electro Marine Systems, Inc.
Energy Analysts, Inc.
Encron, Inc.
Epsco Marine
Eastprint, Inc.
The Ericsson Corporation
European Space Agency
Fluiddyne
Fish Hawk Electronics
Jon Fluke Company
First Mate Marine Autopilots
Franklin Net and Twine, Limited
The Fredericks Company
T.G. Faria Corporation
Fujitsu Ten Corporation of America
Furuno Electric Company (??)The NMEA 0183 Protocol
FUR
GAM
GCA
GES
GFC
GIS
GPI
GRM
GSC
GTO
GVE
GVT
HAL
HAR
HIG
HIT
HPK
HRC
HRT
HTI
HUL
HWM
ICO
IFD
IFI
IME
IMI
IMM
IMP
IMT
INM
INT
IRT
IST
ITM
ITR
JAN
JFR
JMT
JRC
JRI
JTC
JTR
KBE
KBM
KLA
KMR
KNG
KOD
KRP
KVH
KYI
LAT
LEC
LMM
LRD
LSE
LSP
LTF
LWR
MCL
Furuno USA, Inc.
GRE America, Inc.
Gulf Cellular Associates
Geostar Corporation
Graphic Controls Corporation
Galax Integrated Systems
Global Positioning Instrument Corporation
Garmin Corporation
Gold Star Company, Limited
Gro Electronics
Guest Corporation
Great Valley Technology
HAL Communications Corporation
Harris Corporation
Hy-Gain
Hi-Tec
Hewlett-Packard
Harco Manufacturing Company
Hart Systems, Inc.
Heart Interface, Inc.
Hull Electronics Company
Honeywell Marine Systems
Icom of America, Inc.
International Fishing Devices
Instruments for Industry
Imperial Marine Equipment
I.M.I.
ITT MacKay Marine
Impulse Manufacturing, Inc.
International Marketing and Trading, Inc.
Inmar Electronic and Sales, Inc.
Intech, Inc.
Intera Technologies, Ltd.
Innerspace Technology, Inc.
Intermarine Electronics, Inc.
Itera, Limited
Jan Crystals
Ray Jefferson
Japan Marine Telecommunications
Japan Radio Company, Inc.
J-R Industries, Inc.
J-Tech Associates, Inc.
Jotron Radiosearch, Ltd.
KB Electronics, Ld.
Kennebec Marine Company
Klein Associates, Inc.
King Marine Radio Corporation
King Radio Corporation
Koden Electronics Company, Ltd.
Krupp International, Inc.
KVH Company
Kyocera International, Inc.
Latitude Corporation
Lorain Electronics Corporation
Lamarche Manufacturing Company
Lorad
Littlemore Scientific Engineering
Laser Plot, Inc.
Littlefuse, Inc.
Lowrance Electronics Corportation
Micrologic, Inc.
26
MDL
MEC
MEG
MFR
MFW
MGN
MGS
MIE
MIM
MLE
MLN
MLP
MLT
MMB
MME
MMP
MMS
MNI
MNT
MNX
MOT
MPN
MQS
MRC
MRE
MRP
MRR
MRS
MSB
MSE
MSM
MST
MTA
MTG
MTK
MTR
MTS
MUR
MVX
MXX
MES
NAT
NEF
NMR
NGS
NOM
NOV
NSM
NTK
NVC
NVS
NVO
OAR
ODE
ODN
OIN
OKI
OLY
OMN
ORE
Medallion Instruments, Inc.
Marine Engine Center, Inc.
Maritec Engineering GmbH
Modern Products, Ltd
Frank W. Murphy Manufacturing
Magellan Corporation
MG Electronic Sales Corporation
Mieco, Inc.
Marconi International Marine Company
Martha Lake Electronics
Matlin Company
Marlin Products
Miller Technologies
Marsh-McBirney, Inc.
Marks Marine Engineering
Metal Marine Pilot, Inc.
Mars Marine Systems
Micro-Now Instrument Company
Marine Technology
Marinex
Motorola
Memphis Net and Twine Company, Inc.
Marquis Industries, Inc.
Marinecomp, Inc.
Morad Electronics Corporation
Mooring Products of New England
II Morrow, Inc.
Marine Radio Service
Mitsubishi Electric Company, Ltd.
Master Electronics
Master Mariner, Inc.
Mesotech Systems, Ltd.
Marine Technical Associates
Narine Technical Assistance Group
Martech, Inc.
Mitre Corporation, Inc.
Mets, Inc.
Murata Erie North America
Magnavox Advanced Products and
Systems Company
Maxxima Marine
Marine Electronics Service, Inc.
Nautech, Limited
New England Fishing Gear, Inc.
Newmar
Navigation Sciences, Inc.
Nav-Com, Inc.
NovAtel Communications, Ltd.
Northstar Marine
Novatech Designs, Ltd.
Navico
Navstar
Navionics, s.p.a.
O.A.R. Corporation
Ocean Data Equipment Corporation
Odin Electronics, Inc.
Ocean instruments, Inc.
Oki Electronic Industry Company
Navstar Limited (Polytechnic Electronics)
Omnetics
Ocean ResearchThe NMEA 0183 Protocol
OTK
PCE
PDM
PLA
PLI
PMI
PMP
PRK
PSM
PTC
PTG
PTH
RAC
RAE
RAY
RCA
RCH
RCI
RDI
RDM
REC
RFP
RGC
RGY
RMR
RSL
RSM
RWI
RME
RTN
SAI
SBR
SCR
SEA
SEC
SEP
SFN
SGC
SIG
SIM
SKA
SKP
SLI
SME
SMF
SML
SMI
SNV
SOM
SOV
SPL
SPT
SRD
SRS
SRT
SSI
STC
STI
STM
SVY
SWI
Ocean Technology
Pace
Prodelco Marine Systems
Plath, C. Division of Litton
Pilot Instruments
Pernicka Marine Products
Pacific Marine Products
Perko, Inc.
Pearce-Simpson
Petro-Com
P.T.I./Guest
Pathcom, Inc.
Racal Marine, Inc.
RCA Astro-Electronics
Raytheon Marine Company
RCA Service Company
Roach Engineering
Rochester Instruments, Inc.
Radar Devices
Ray-Dar Manufacturing Company
Ross Engineering Company
Rolfite Products, Inc.
RCS Global Communications, Inc.
Regency Electronics, Inc.
RCA Missile and Surface Radar
Ross Laboratories, Inc.
Robertson-Shipmate, USA
Rockwell International
Racal Marine Electronics
Robertson Tritech Nyaskaien A/S
SAIT, Inc.
Sea-Bird electronics, Inc.
Signalcrafters, Inc.
SEA
Sercel Electronics of Canada
Steel and Engine Products, Inc.
Seafarer Navigation International
SGC, Inc.
Signet, Inc.
Simrad,Inc
Skantek Corporation
Skipper Electronics A/S
Starlink, Inc.
Shakespeare Marine Electronics
Seattle Marine and Fishing Supply Co.
Simerl Instruments
Sperry Marine, Inc.
Starnav Corporation
Sound Marine Electronics, Inc.
Sell Overseas America
Spelmar
Sound Powered Telephone
SRD Labs
Scientific Radio Systems, Inc.
Standard Radio and Telefon AB
Sea Scout Industries
Standard Communications
Sea-Temp Instrument Corporation
Si-Tex Marine Electronics
Savoy Electronics
Swoffer Marine Instruments, Inc.
27
SRS
TBB
TCN
TDL
THR
TLS
TMT
TNL
TRC
TSI
TTK
TTS
TWC
TXI
UME
UNI
UNP
UNF
VAN
VAR
VCM
VEX
VIS
VMR
WAL
WBG
WEC
WHA
WMM
WMR
WNG
WSE
WTC
WST
YAS
Shipmate, Rauff & Sorensen, A/S
Thompson Brothers Boat Manufacturing
Company
Trade Commission of Norway (THE)
Tideland Signal
Thrane and Thrane A/A
Telesystems
Tamtech, Ltd.
Trimble Navigation
Tracor, Inc.
Techsonic Industries, Inc.
Talon Technology Corporation
Transtector Systems
Transworld Communications, Inc.
Texas Instruments, Inc.
Umec
Uniden Corporation of America
Unipas, Inc.
Uniforce Electronics Company
Vanner, Inc.
Varian Eimac Associates
Videocom
Vexillar
Vessel Information Systems, Inc.
Vast Marketing Corporation
Walport USA
Westberg Manufacturing, Inc.
Westinghouse Electric Corporation
W-H Autopilots
Wait Manufacturing and Marine Sales
Company
Wesmar Electronics
Winegard Company
Wilson Electronics Corporation
Watercom
West Electronics Ltd.
Yaesu ElectronicsThe NMEA 0183 Protocol
28
8. References
[1] National Marine Electronics Association: http://www.nmea.org
[2] Torsten Baumbach's web site: http://pandora.inf.uni-jena.de/ttbb/
[3] Peter Bennett’s GPS and NMEA site: http://vancouver-webpages.com/pub/peter/index.html
[4] Data Transmission Protocol Specification for Magellan Products. Revision 1.0. Magellan Corporation,
Santa Clara 1999. Available at: http://magellangps.com
This document describes the protocol used by Magellan’s consumer GPS units, including a number
of NMEA 0183 proprietary sentences.
[5]
SBA-1 Interfacing Manual. Revision 0.0. Communications Systems International, Inc, Calgary, 1999.
Available at: www.csi-dgps.com.
This manual explains the interfacing of the SBA-1 DGPS beacon receiver to numerous GPS units as
well as the CSI proprietary sentences used.