import React from 'react';
import Button from '../../components/button';
import Checkbox from '../../components/checkbox'
import Text from '../../components/text';
import { ContainerBody } from './conectarWiFi.styled'

const ConectarWiFi: React.FC = () => {
  return (
    <ContainerBody>
      <h1>Conectar na Internet</h1>
      <Button>AUTOMATICO</Button>
      <Button>Manual</Button>

      <Checkbox
        id='conectar'
        onChange={() => { }}
        ><Text>Não conectar no WiFi.</Text></Checkbox>

      <Button>OK</Button>
    </ContainerBody>
  );
}

export default ConectarWiFi;