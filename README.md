## Face Detector and Socket Server

**Software desenvolvido para a disciplinas de Sistemas Embarcados**

*Baseado em:*

- http://beej.us/guide/bgnet/html/single/bgnet.html#simpleserver
- https://developer.toradex.com/knowledge-base/opencv-(linux)

## Compilaçao

### Dockerfile

O projeto utiliza um container para subir o servidor socket que responde a requisicoes de
reconhecimento de face. Para fazer uso do container, basta, no sistema que será o servidor,
rodar, na pasta que contem a Dockerfile:

```
docker build -t face_rec-socket .
```

e depois 

```
docker run -it --privileged -v /dev:/dev -p <porta-socket>:<porta-socket> face_rec-socket bash
```

### Compilando na mão

**Versoes:**

| Pacote | Versao |
| ------ | ------ |
| OpenCV | 3.2.0 |
| g++ | 7.4.0 |

**Compilando com as libs do OpenCV:**

1. Mude as variaveis `PORT` e `face_cascade_name` para a porta a ser utilizada na
comunicacao dos sockets e para o arquivo que contem as cascades de face detection.
Na minha placa, o caminho e `/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml`.

2. Mude também a entrada do vídeo. Em arquitetura x86, provavelmente será algo
como `capture.open(0)`, indicando a webcam do laptop, ou `/dev/X`, `/dev/videoX`,...
Para a placa ARM, será algo como: `v4l2:///dev/videoX`.

3. Compile o programa com as libs e flags do OpenCV

```
$ g++ main.cpp -o main `pkg-config --cflags --libs opencv`
```

## Utilizacao

1. Inicie o servidor socket-image_recognition, tanto rodando o programa compilado
ou subindo o container Docker

2. Rode o socket client em qualquer lugar (ex: telnet, esp8266, etc)

## Next Stuff

- [ ] Build image and toolchain for cross-compiling
- [x] Setup NodeMCU to be the socket client
- [ ] Fix Bug: You have to ping the socket server 5 times before getting an updated image
______

**EESC 2019**
