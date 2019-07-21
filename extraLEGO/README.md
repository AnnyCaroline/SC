# Material extra para o LEGO

Dias para a programação Lego

1. Em anexo, dois gerenciadores para download do LabVIEW e do toolkit NXT/EV3 e os blocos específicos para controle dos sensores de som e ultrassom na plataforma EV3.

2. Foi verificado um problema com o sensor de luz. Parece que a versão nova do software EV3 não apresenta um bloco para o sensor de luz, apenas para sensor de cor causando um erro ao utilizá-lo para obter as amostras do sensor de luz.

Para resolver isso basta utilizar o bloco Raw Sensor Value identificando a porta do sensor correspondente (3) e efetuar uma conversão para porcentagem dividindo por 10 e truncando o número (conversão para inteiro).

Assim os valores recebidos ficam em torno de 40 a 60, caso utilize uma iluminação ambiente muito alto ele abaixa para quase 10. Imagem dos blocos necessários a seguir.
