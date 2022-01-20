import React from 'react';
import Button from '../../components/button';
import Checkbox from '../../components/checkbox'
import Text from '../../components/text';
import { ContainerBody } from './conectarWiFi.styled'

const ConectarWiFi: React.FC = () => {
  return (
    <ContainerBody>
      <h1><Text fontSize='30px'>Conectar na Internet</Text></h1>
      <Button cursor= "pointer"><Text fontSize='18px'>Automático</Text></Button>
      <Button cursor= "pointer"><Text fontSize='18px'>Manual</Text></Button>

      <Checkbox
        id='conectar'
        onChange={() => { }}
        ><Text fontSize='18px'>Não conectar no WiFi.</Text></Checkbox>

      <Button cursor= "pointer"><Text fontSize='18px'>Confirmar</Text></Button>
    </ContainerBody>
  );
}

export default ConectarWiFi;