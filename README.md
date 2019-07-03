# Sistema de reconhecimento de faces para uso residencial*

Este projeto consiste em desenvolver um sistema de detecção de faces utilizando uma WebCam.
O projeto foi pensado para, inicialmente, ser aplicado em automação residencial, sendo
utilizado como uma forma de aviso para o morador quando alguém chega na porta de sua casa.
Sua arquitetura é dividida basicamente em duas partes: o **servidor**, que é constituido por 
uma placa da Toradex e o **cliente**, que é constituido por um módulo WiFi ESP8266. A seguir,
vemos sua arquitetura detalhada.

## A arquitetura

Uma WebCam é conectada à placa Toradex, que, por meio da biblioteca OpenCV, faz o reconhecimento
de faces. Também na placa, há instalado um servidor socket que utiliza o protocolo TCP para a 
realizar comunicacão com o roteador que, por sua vez, se comunica com o módulo WiFi.
Conectado ao módulo Wifi, há um atuador, que, para fins demonstrativos, é um servo motor.
Quando detectada alguma face, o motor gira. Este atuador representa um sistema de alerta, que 
ficará sob posse do morador da residência, podendo ser substituído por um smartwatch ou aplicativo
de celular futuramente.

## Face Detector and Socket Server

**Software desenvolvido para a disciplinas de Sistemas Embarcados**

*Baseado em:*

- http://beej.us/guide/bgnet/html/single/bgnet.html#simpleserver
- https://developer.toradex.com/knowledge-base/opencv-(linux)

## Compilação

### Dockerfile

O projeto utiliza um container para subir o servidor socket que responde a requisicoes de
reconhecimento de face. Para fazer uso do container, basta, no sistema que será o servidor,
rodar, na pasta que contem a Dockerfile:

```
docker build -t face_rec-socket .
```

e depois 

```
docker run -it --privileged -v /dev:/dev -p 3945:3945 face_rec-socket bash
```

### Compilando na mão

**Versões:**

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

## Utilização

1. Inicie o servidor socket-image_recognition, tanto rodando o programa compilado
ou subindo o container Docker

2. Rode o socket client em qualquer lugar (ex: telnet, esp8266, etc)

## Next Stuff

- [ ] Build image and toolchain for cross-compiling
- [x] Setup NodeMCU to be the socket client
- [ ] Fix Bug: You have to ping the socket server 5 times before getting an updated image
______
## Integrantes do Projeto

- Anne Sanflorian Pretyman
- Henrique Toledo Amâncio Silva
- Lucas Hehl Pinto Ferraz
- Márcio Kazuaki Tamada

## Vídeo-Relatório

- https://drive.google.com/file/d/1pOkyNenUZsf9oktheKlJQFWGM7NC-P4V/view?usp=sharing
______

**EESC 2019**
