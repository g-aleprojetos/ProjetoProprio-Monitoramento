import styled from 'styled-components'

export interface Props {
  id?: string;
  name?: string;
  label?: string;
  placeholder?: string;
  autoComplete?: 'on' | 'off';
  type?: 'Text';
  onChange?: (e: any) => void | (() => void);

}
export const Container = styled.div`
  display: flex;
  flex-direction: column;
  //padding: 25px;
 // position: relative;
`;

export const Label = styled.label`
  color: #000;
  font-weight: normal;
  opacity: 0.75;
  order: 1;
  /*outline: 0;*/
  padding-left: 2px;
  pointer-events: none;
  text-shadow: none;
  text-transform: capitalize;
  transform-origin: left top;
  transform: scale(1) translate3d(0, 22px, 0);
  transition: 200ms ease all;
`;

export const StyleInput = styled.input`
  border-radius: 0;
  display: flex;
  font-size: 100%;
  line-height: 25px;
  text-shadow: none;

  border: 0;
  border-bottom: 1px solid rgba(0, 0, 0, 0.15);
  color: #000;
  flex: 1 1 auto;
  order: 2;

  &:focus {
  outline: 0;
  }

  &:not(:focus) {
    color: transparent;
  }

  &:focus + ${Label} {
    color: #3949AB;
    opacity: 1;
    transform: scale(0.8) translate3d(0, 5px, 0);
  }
`;

