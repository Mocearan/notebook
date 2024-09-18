# rtmp-capture

---

​		抓包分析RTMP



## 推送

![image-20240918215552605](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240918215552605.png)

- `c0 + c1`

  ```shell
  # 03 00 00 00 00 09 00 7c 02 f7 78 55 1e ce ab 8e
  
  Handshake C0+C1
      Protocol version: 03
      Handshake data [truncated]:0000000009007c02f77855...
  ```

- `s0 + s1 + s2`

  ```shell
  # 0000   03 be 0e 85 c4 0d 0e 0a 0d d6 b6 47 a8 55 5b f4
  
  Handshake S0+S1+S2
  	Protocol version: 03
      Handshake data [truncated]: be0e85c40d0e0a0dd6b647a8555bf420...
      Handshake data [truncated]: 6277d56adbc51935eb6b801a44a36f4c...
  ```

- `c2`

  ```shell
  # 0000   be 0e 85 c4 0d 0e 0a 0d d6 b6 47 a8 55 5b f4 20
  
  Handshake C2
      Handshake data [truncated]: be0e85c40d0e0a0dd6b647
  ```

- `connect('live')`

  ```shell
  #0000   03 00 00 00 00 00 90 14 00 00 00 00 |【02】 00 07 63
  #0010   6f 6e 6e 65 63 74 | 【00】 3f f0 00 00 00 00 00 00 | 【03】
  #0020   00 03 61 70 70 02 00 04 6c 69 76 65 | 00 04 74 79
  #0030   70 65 02 00 0a 6e 6f 6e 70 72 69 76 61 74 65 | 00
  #0040   08 66 6c 61 73 68 56 65 72 02 00 24 46 4d 4c 45
  #0050   2f 33 2e 30 20 28 63 6f 6d 70 61 74 69 62 6c 65
  #0060   3b 20 4c 61 76 66 36 30 2e 31 36 2e 31 30 30 29
  #0070   00 05 74 63 55 72 6c 02 00 1f 72 74 6d 70 3a 2f
  #0080   2f 31 32 34 2e 32 32 32 2e 37 33 2e 32 33 31 3a
  #0090   31 39 33 35 2f 6c 69 76 65 00 00 09
  
  
  Real Time Messaging Protocol (AMF0 Command connect('live'))
      Response to this call in frame: 825
      RTMP Header
          00.. .... = Format: 0
          ..00 0011 = Chunk Stream ID: 3
          Timestamp: 0
          Body size: 144
          Type ID: AMF0 Command (0x14)
          Stream ID: 0
      RTMP Body
          String 'connect'
              AMF0 type: String (0x02)
              String length: 7
              String: connect
          Number 1
              AMF0 type: Number (0x00)
              Number: 1
          Object (4 items)
              AMF0 type: Object (0x03)
              Property 'app' String 'live'
                  Name: app
                      String length: 3
                      String: app
                  String 'live'
                      AMF0 type: String (0x02)
                      String length: 4
                      String: live
              Property 'type' String 'nonprivate'
                  Name: type
                      String length: 4
                      String: type
                  String 'nonprivate'
                      AMF0 type: String (0x02)
                      String length: 10
                      String: nonprivate
              Property 'flashVer' String 'FMLE/3.0 (compatible; Lavf60.16.100)'
                  Name: flashVer
                      String length: 8
                      String: flashVer
                  String 'FMLE/3.0 (compatible; Lavf60.16.100)'
                      AMF0 type: String (0x02)
                      String length: 36
                      String: FMLE/3.0 (compatible; Lavf60.16.100)
              Property 'tcUrl' String 'rtmp://124.222.73.231:1935/live'
                  Name: tcUrl
                      String length: 5
                      String: tcUrl
                  String 'rtmp://124.222.73.231:1935/live'
                      AMF0 type: String (0x02)
                      String length: 31
                      String: rtmp://124.222.73.231:1935/live
              End Of Object Marker
  ```

- `window acknowledgement size`

  ```shell
  # 0000   02 00 00 00 00 00 04 05 00 00 00 00 00 4c 4b 40
  
  Real Time Messaging Protocol (Window Acknowledgement Size 5000000)
      RTMP Header
          00.. .... = Format: 0
          ..00 0010 = Chunk Stream ID: 2
          Timestamp: 0
          Body size: 4
          Type ID: Window Acknowledgement Size (0x05)
          Stream ID: 0
      RTMP Body
          Window acknowledgement size: 5000000
  ```

- `set peer bandwidth / set chunk size / _result('NetConnection.Connect.Success')`

  - `set peer bandwidth`

    ```shell
    # 0000   02 00 00 00 00 00 05 06 00 00 00 00 00 4c 4b 40
    # 0010   02
    
    Real Time Messaging Protocol (Set Peer Bandwidth 5000000,Dynamic)
        RTMP Header
            00.. .... = Format: 0
            ..00 0010 = Chunk Stream ID: 2
            Timestamp: 0
            Body size: 5
            Type ID: Set Peer Bandwidth (0x06)
            Stream ID: 0
        RTMP Body
            Window acknowledgement size: 5000000
            Limit type: Dynamic (2)
    ```

  - `set chunk size`

    ```shell
    # 0000   02 00 00 00 00 00 04 01 00 00 00 00 00 00 10 00
    
    Real Time Messaging Protocol (Set Chunk Size 4096)
        RTMP Header
            00.. .... = Format: 0
            ..00 0010 = Chunk Stream ID: 2
            Timestamp: 0
            Body size: 4
            Type ID: Set Chunk Size (0x01)
            Stream ID: 0
        RTMP Body
            Chunk size: 4096
    ```

  - ` _result('NetConnection.Connect.Success`

    ```shell
    #0000   03 00 00 00 00 00 be 14 00 00 00 00 02 00 07 5f
    #0010   72 65 73 75 6c 74 00 3f f0 00 00 00 00 00 00 03
    #0020   00 06 66 6d 73 56 65 72 02 00 0d 46 4d 53 2f 33
    #0030   2c 30 2c 31 2c 31 32 33 00 0c 63 61 70 61 62 69
    #0040   6c 69 74 69 65 73 00 40 3f 00 00 00 00 00 00 00
    #0050   00 09 03 00 05 6c 65 76 65 6c 02 00 06 73 74 61
    #0060   74 75 73 00 04 63 6f 64 65 02 00 1d 4e 65 74 43
    #0070   6f 6e 6e 65 63 74 69 6f 6e 2e 43 6f 6e 6e 65 63
    #0080   74 2e 53 75 63 63 65 73 73 00 0b 64 65 73 63 72
    #0090   69 70 74 69 6f 6e 02 00 15 43 6f 6e 6e 65 63 74
    #00a0   69 6f 6e 20 73 75 63 63 65 65 64 65 64 2e 00 0e
    #00b0   6f 62 6a 65 63 74 45 6e 63 6f 64 69 6e 67 00 00
    #00c0   00 00 00 00 00 00 00 00 00 09
    
    Real Time Messaging Protocol (AMF0 Command _result('NetConnection.Connect.Success'))
        Call for this response in frame: 794
        RTMP Header
            00.. .... = Format: 0
            ..00 0011 = Chunk Stream ID: 3
            Timestamp: 0
            Body size: 190
            Type ID: AMF0 Command (0x14)
            Stream ID: 0
        RTMP Body
            String '_result'
                AMF0 type: String (0x02)
                String length: 7
                String: _result
            Number 1
                AMF0 type: Number (0x00)
                Number: 1
            Object (2 items)
                AMF0 type: Object (0x03)
                Property 'fmsVer' String 'FMS/3,0,1,123'
                    Name: fmsVer
                        String length: 6
                        String: fmsVer
                    String 'FMS/3,0,1,123'
                        AMF0 type: String (0x02)
                        String length: 13
                        String: FMS/3,0,1,123
                Property 'capabilities' Number 31
                    Name: capabilities
                        String length: 12
                        String: capabilities
                    Number 31
                        AMF0 type: Number (0x00)
                        Number: 31
                End Of Object Marker
            Object (4 items)
                AMF0 type: Object (0x03)
                Property 'level' String 'status'
                    Name: level
                        String length: 5
                        String: level
                    String 'status'
                        AMF0 type: String (0x02)
                        String length: 6
                        String: status
                Property 'code' String 'NetConnection.Connect.Success'
                    Name: code
                        String length: 4
                        String: code
                    String 'NetConnection.Connect.Success'
                        AMF0 type: String (0x02)
                        String length: 29
                        String: NetConnection.Connect.Success
                Property 'description' String 'Connection succeeded.'
                    Name: description
                        String length: 11
                        String: description
                    String 'Connection succeeded.'
                        AMF0 type: String (0x02)
                        String length: 21
                        String: Connection succeeded.
                Property 'objectEncoding' Number 0
                    Name: objectEncoding
                        String length: 14
                        String: objectEncoding
                    Number 0
                        AMF0 type: Number (0x00)
                        Number: 0
                End Of Object Marker
    ```

- `set chunk size / releaseStream('room') / FCPublish('room') / createStream()`

  - `set chunk size`

    ```shell
    # 0000   02 00 00 00 00 00 04 01 00 00 00 00 00 00 10 00
    
    Real Time Messaging Protocol (Set Chunk Size 4096)
        RTMP Header
            00.. .... = Format: 0
            ..00 0010 = Chunk Stream ID: 2
            Timestamp: 0
            Body size: 4
            Type ID: Set Chunk Size (0x01)
            Stream ID: 0
        RTMP Body
            Chunk size: 4096
    ```

  - `releaseStream('room')`

    ```shell
    #0000   43 00 00 00 00 00 21 14 02 00 0d 72 65 6c 65 61
    #0010   73 65 53 74 72 65 61 6d 00 40 00 00 00 00 00 00
    #0020   00 05 02 00 04 72 6f 6f 6d
    
    Real Time Messaging Protocol (AMF0 Command releaseStream('room'))
        RTMP Header
            01.. .... = Format: 1
            ..00 0011 = Chunk Stream ID: 3
            Timestamp delta: 0
            Timestamp: 0 (calculated)
            Body size: 33
            Type ID: AMF0 Command (0x14)
        RTMP Body
            String 'releaseStream'
                AMF0 type: String (0x02)
                String length: 13
                String: releaseStream
            Number 2
                AMF0 type: Number (0x00)
                Number: 2
            Null
                AMF0 type: Null (0x05)
            String 'room'
                AMF0 type: String (0x02)
                String length: 4
                String: room
    ```

    

  - `FCPublish('room')`

    ```shell
    #0000   43 00 00 00 00 00 1d 14 02 00 09 46 43 50 75 62
    #0010   6c 69 73 68 00 40 08 00 00 00 00 00 00 05 02 00
    #0020   04 72 6f 6f 6d
    
    Real Time Messaging Protocol (AMF0 Command FCPublish('room'))
        RTMP Header
            01.. .... = Format: 1
            ..00 0011 = Chunk Stream ID: 3
            Timestamp delta: 0
            Timestamp: 0 (calculated)
            Body size: 29
            Type ID: AMF0 Command (0x14)
        RTMP Body
            String 'FCPublish'
                AMF0 type: String (0x02)
                String length: 9
                String: FCPublish
            Number 3
                AMF0 type: Number (0x00)
                Number: 3
            Null
                AMF0 type: Null (0x05)
            String 'room'
                AMF0 type: String (0x02)
                String length: 4
                String: room
    ```

  - `createStream()`

    ```shell
    #0000   43 00 00 00 00 00 19 14 02 00 0c 63 72 65 61 74
    #0010   65 53 74 72 65 61 6d 00 40 10 00 00 00 00 00 00
    #0020   05
    
    Real Time Messaging Protocol (AMF0 Command createStream())
        Response to this call in frame: 848
        RTMP Header
            01.. .... = Format: 1
            ..00 0011 = Chunk Stream ID: 3
            Timestamp delta: 0
            Timestamp: 0 (calculated)
            Body size: 25
            Type ID: AMF0 Command (0x14)
        RTMP Body
            String 'createStream'
                AMF0 type: String (0x02)
                String length: 12
                String: createStream
            Number 4
                AMF0 type: Number (0x00)
                Number: 4
            Null
                AMF0 type: Null (0x05)
    ```

- `_result()`

  ```shell
  0000   03 00 00 00 00 00 1d 14 00 00 00 00 02 00 07 5f
  0010   72 65 73 75 6c 74 00 40 10 00 00 00 00 00 00 05
  0020   00 3f f0 00 00 00 00 00 00
  
  Real Time Messaging Protocol (AMF0 Command _result())
      Call for this response in frame: 838
      RTMP Header
          00.. .... = Format: 0
          ..00 0011 = Chunk Stream ID: 3
          Timestamp: 0
          Body size: 29
          Type ID: AMF0 Command (0x14)
          Stream ID: 0
      RTMP Body
          String '_result'
              AMF0 type: String (0x02)
              String length: 7
              String: _result
          Number 4
              AMF0 type: Number (0x00)
              Number: 4
          Null
              AMF0 type: Null (0x05)
          Number 1
              AMF0 type: Number (0x00)
              Number: 1
  ```

- `publish('room')`

  ```shell
  0000   08 00 00 00 00 00 22 14 01 00 00 00 02 00 07 70
  0010   75 62 6c 69 73 68 00 40 14 00 00 00 00 00 00 05
  0020   02 00 04 72 6f 6f 6d 02 00 04 6c 69 76 65
  
  Real Time Messaging Protocol (AMF0 Command publish('room'))
      RTMP Header
          00.. .... = Format: 0
          ..00 1000 = Chunk Stream ID: 8
          Timestamp: 0
          Body size: 34
          Type ID: AMF0 Command (0x14)
          Stream ID: 1
      RTMP Body
          String 'publish'
              AMF0 type: String (0x02)
              String length: 7
              String: publish
          Number 5
              AMF0 type: Number (0x00)
              Number: 5
          Null
              AMF0 type: Null (0x05)
          String 'room'
              AMF0 type: String (0x02)
              String length: 4
              String: room
          String 'live'
              AMF0 type: String (0x02)
              String length: 4
              String: live
  ```

- `onStatus('NetStream.Publish.Start')`

  ```shell
  0000   05 00 00 00 00 00 69 14 01 00 00 00 02 00 08 6f
  0010   6e 53 74 61 74 75 73 00 00 00 00 00 00 00 00 00
  0020   05 03 00 05 6c 65 76 65 6c 02 00 06 73 74 61 74
  0030   75 73 00 04 63 6f 64 65 02 00 17 4e 65 74 53 74
  0040   72 65 61 6d 2e 50 75 62 6c 69 73 68 2e 53 74 61
  0050   72 74 00 0b 64 65 73 63 72 69 70 74 69 6f 6e 02
  0060   00 10 53 74 61 72 74 20 70 75 62 6c 69 73 68 69
  0070   6e 67 00 00 09
  
  Real Time Messaging Protocol (AMF0 Command onStatus('NetStream.Publish.Start'))
      RTMP Header
          00.. .... = Format: 0
          ..00 0101 = Chunk Stream ID: 5
          Timestamp: 0
          Body size: 105
          Type ID: AMF0 Command (0x14)
          Stream ID: 1
      RTMP Body
          String 'onStatus'
              AMF0 type: String (0x02)
              String length: 8
              String: onStatus
          Number 0
              AMF0 type: Number (0x00)
              Number: 0
          Null
              AMF0 type: Null (0x05)
          Object (3 items)
              AMF0 type: Object (0x03)
              Property 'level' String 'status'
                  Name: level
                      String length: 5
                      String: level
                  String 'status'
                      AMF0 type: String (0x02)
                      String length: 6
                      String: status
              Property 'code' String 'NetStream.Publish.Start'
                  Name: code
                      String length: 4
                      String: code
                  String 'NetStream.Publish.Start'
                      AMF0 type: String (0x02)
                      String length: 23
                      String: NetStream.Publish.Start
              Property 'description' String 'Start publishing'
                  Name: description
                      String length: 11
                      String: description
                  String 'Start publishing'
                      AMF0 type: String (0x02)
                      String length: 16
                      String: Start publishing
              End Of Object Marker
  ```

- `@setDataFrame`

  ```shell
  0000   04 00 00 00 00 01 27 12 01 00 00 00 02 00 0d 40
  0010   73 65 74 44 61 74 61 46 72 61 6d 65 02 00 0a 6f
  0020   6e 4d 65 74 61 44 61 74 61 08 00 00 00 0a 00 08
  0030   64 75 72 61 74 69 6f 6e 00 00 00 00 00 00 00 00
  0040   00 00 05 77 69 64 74 68 00 40 94 00 00 00 00 00
  0050   00 00 06 68 65 69 67 68 74 00 40 86 80 00 00 00
  0060   00 00 00 0d 76 69 64 65 6f 64 61 74 61 72 61 74
  0070   65 00 40 d3 12 d0 00 00 00 00 00 09 66 72 61 6d
  0080   65 72 61 74 65 00 40 2e 00 00 00 00 00 00 00 0c
  0090   76 69 64 65 6f 63 6f 64 65 63 69 64 00 40 00 00
  00a0   00 00 00 00 00 00 05 74 69 74 6c 65 02 00 1a 48
  00b0   49 4b 20 4d 65 64 69 61 20 53 65 72 76 65 72 20
  00c0   56 34 2e 33 30 2e 30 38 35 00 07 63 6f 6d 6d 65
  00d0   6e 74 02 00 2f 48 49 4b 20 4d 65 64 69 61 20 53
  00e0   65 72 76 65 72 20 53 65 73 73 69 6f 6e 20 44 65
  00f0   73 63 72 69 70 74 69 6f 6e 20 3a 20 73 74 61 6e
  0100   64 61 72 64 00 07 65 6e 63 6f 64 65 72 02 00 0d
  0110   4c 61 76 66 36 30 2e 31 36 2e 31 30 30 00 08 66
  0120   69 6c 65 73 69 7a 65 00 00 00 00 00 00 00 00 00
  0130   00 00 09
  
  Real Time Messaging Protocol (AMF0 Data @setDataFrame())
      RTMP Header
          00.. .... = Format: 0
          ..00 0100 = Chunk Stream ID: 4
          Timestamp: 0
          Body size: 295
          Type ID: AMF0 Data (0x12)
          Stream ID: 1
      RTMP Body
          String '@setDataFrame'
              AMF0 type: String (0x02)
              String length: 13
              String: @setDataFrame
          String 'onMetaData'
              AMF0 type: String (0x02)
              String length: 10
              String: onMetaData
          ECMA array (10 items)
              AMF0 type: ECMA array (0x08)
              Array length: 10
              Property 'duration' Number 0
                  Name: duration
                      String length: 8
                      String: duration
                  Number 0
                      AMF0 type: Number (0x00)
                      Number: 0
              Property 'width' Number 1280
                  Name: width
                      String length: 5
                      String: width
                  Number 1280
                      AMF0 type: Number (0x00)
                      Number: 1280
              Property 'height' Number 720
                  Name: height
                      String length: 6
                      String: height
                  Number 720
                      AMF0 type: Number (0x00)
                      Number: 720
              Property 'videodatarate' Number 19531.25
                  Name: videodatarate
                      String length: 13
                      String: videodatarate
                  Number 19531.25
                      AMF0 type: Number (0x00)
                      Number: 19531.25
              Property 'framerate' Number 15
                  Name: framerate
                      String length: 9
                      String: framerate
                  Number 15
                      AMF0 type: Number (0x00)
                      Number: 15
              Property 'videocodecid' Number 2
                  Name: videocodecid
                      String length: 12
                      String: videocodecid
                  Number 2
                      AMF0 type: Number (0x00)
                      Number: 2
              Property 'title' String 'HIK Media Server V4.30.085'
                  Name: title
                      String length: 5
                      String: title
                  String 'HIK Media Server V4.30.085'
                      AMF0 type: String (0x02)
                      String length: 26
                      String: HIK Media Server V4.30.085
              Property 'comment' String 'HIK Media Server Session Description : standard'
                  Name: comment
                      String length: 7
                      String: comment
                  String 'HIK Media Server Session Description : standard'
                      AMF0 type: String (0x02)
                      String length: 47
                      String: HIK Media Server Session Description : standard
              Property 'encoder' String 'Lavf60.16.100'
                  Name: encoder
                      String length: 7
                      String: encoder
                  String 'Lavf60.16.100'
                      AMF0 type: String (0x02)
                      String length: 13
                      String: Lavf60.16.100
              Property 'filesize' Number 0
                  Name: filesize
                      String length: 8
                      String: filesize
                  Number 0
                      AMF0 type: Number (0x00)
                      Number: 0
              End Of Object Marker
  
  ```

- `Video Data`

  ```shell
  0000   06 00 00 00 00 71 1c 09 01 00 00 00 12 00 00 84
  
  Real Time Messaging Protocol (Video Data)
      RTMP Header
          00.. .... = Format: 0
          ..00 0110 = Chunk Stream ID: 6
          Timestamp: 0
          Body size: 28956
          Type ID: Video Data (0x09)
          Stream ID: 1
      RTMP Body
      	Control: 0x12 (keyframe Sorensen H.263)
              0001 .... = Type: keyframe (1)
              .... 0010 = Format: Sorensen H.263 (2)
  			Video data [truncated]: 000084008280016811a6fcfcfcfdffff37e7e7e7effff9bf3f3f3f7fffcdf9f...
  ```
  
  - 这个好像不太对，不知道是不是因为H263没有SPS、PPS





