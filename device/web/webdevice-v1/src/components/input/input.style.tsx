import styled from 'styled-components'

export interface Props { 
  id?: string;
  name?: string;  
  label?: string;
  placeholder?: string;
  autoComplete?: 'on' | 'off';
  type?: 'checkbox' | 'Text';
  
  
}
export const Container = styled.div`
width: 200px;
height: 20px;
`;

export const StyleInput = styled.input`

`;

export const StyleLabel = styled.label`

`;