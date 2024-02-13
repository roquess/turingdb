import { InputGroup, InputGroupProps } from '@blueprintjs/core'
import clsx from 'clsx'

export interface AppInputProps extends InputGroupProps {
  small?: boolean
}

export default function AppInput(props: AppInputProps) {
  return (
    <InputGroup
      {...props}
      className={clsx(
        'app-input',
        {
          'app-input--small': props.small,
        },
        props.className
      )}
    />
  )
}
