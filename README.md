## Face Detector and Socket Server

**Software desenvolvido para a disciplinas de Sistemas Embarcados**

*Baseado em:*

- http://beej.us/guide/bgnet/html/single/bgnet.html#simpleserver
- https://developer.toradex.com/knowledge-base/opencv-(linux)

## Compilaçao e Uso

**Versoes:**

| Pacote | Versao |
| ------ | ------ |
| OpenCV | 3.2.0 |
| g++ | 7.4.0 |

**Compilando com as libs do OpenCV:**

1. Mude as variaveis `PORT` e `face_cascade_name` para a porta a ser utilizada na
comunicacao dos sockets e para o arquivo que contem as cascades de face detection.
No meu pc, o caminho e `/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml`.

2. Mude também a entrada do vídeo. Em arquitetura x86, provavelmente será algo
como `capture.open(0)`, indicando a webcam do laptop, ou `/dev/X`, `/dev/videoX`,...
Para a placa ARM, será algo como: `v4l2:///dev/videoX`.

3. Compile o programa com as libs e flags do OpenCV

```
$ g++ main.cpp -o main `pkg-config --cflags --libs opencv`
```

**Uso**

1. Roda o `./main` na placa (image recognition & servidor socket)

2. Roda o socket client em qualquer lugar (ex: telnet, esp8266, etc)

## Next Stuff

- [ ] Build image and toolchain for cross-compiling
- [ ] Setup NodeMCU to be the socket client
- [ ] Fix Bug: You have to ping the socket server 5 times before getting an updated image
______

**EESC 2019**
