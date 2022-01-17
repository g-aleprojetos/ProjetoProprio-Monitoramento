import React from 'react';
import Button from '../../components/button';
import Input from '../../components/input';
import {ContainerBody} from './conectarWiFi.styled'

const ConectarWiFi: React.FC = () =>{
  return(
    <ContainerBody>
    <h1>Conectar na Internet</h1>
    <Button>AUTOMATICO</Button>
    <Button>Manual</Button>
    <Input id='conectar'/>
    <Button>OK</Button>
    </ContainerBody>
  );
}

export default ConectarWiFi;