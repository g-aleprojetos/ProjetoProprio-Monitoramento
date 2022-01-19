import React from 'react'
import { Container, StyleInput,Props, Label } from './input.style';

const Input: React.FC<Props> = ({...props}) => {
  return (
    <Container>
      <StyleInput {...props} />
      <Label >{props.placeholder}</Label>
    </Container>
  );
};

export default Input;