import React from 'react';
import { Container, Label, Props, StyledCheckout } from './checkbox.styles';

const Checkbox: React.FC<Props> = ({children, ...props }) => {
  return (
    <Container>
      <Label>
        <StyledCheckout type={'checkbox'}{...props} />
        {children}
      </Label>
    </Container>
  )
}

export default Checkbox;