import React from 'react'
import { Container, StyleInput,Props, StyleLabel } from './input.style';

const Input: React.FC<Props> = ({...props}) => {
  return (
    <Container>
      <StyleInput {...props} />
      <StyleLabel for={props.label}>{props.placeholder}</StyleLabel>
    </Container>
  );
};

export default Input;