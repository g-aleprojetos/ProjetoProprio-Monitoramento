import styled from "styled-components";

export interface Props {
  id?: string;
  name?: string;
  label?: string;
  children?: React.ReactNode;
   
  onChange?: (e: any) => void | (() => void);
}

export const Container = styled.div`
margin: 10px;
padding: 10px;
`;

export const StyledCheckout = styled.input``;

export const Label = styled.label`
cursor: pointer;
`;