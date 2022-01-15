import styled from 'styled-components'

export interface Props{
  id?: string;
  name?: string;
  type?: "button" | "submit" | "reset";
  disabled?: true | false;
  width?: string;
  height?: string;
  children?: string | React.ReactNode;
}

export const StyleSheet = styled.button<Props>`
width:${props => props.width ?? '200px'};
height: ${props => props.height ?? '30px'};
border-radius: 5px;
cursor: pointer;
`