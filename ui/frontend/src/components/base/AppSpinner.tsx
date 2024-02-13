import { Spinner, SpinnerProps } from '@blueprintjs/core'
import clsx from 'clsx'

export interface AppSpinnerProps extends SpinnerProps {}

export default function AppSpinner(props: AppSpinnerProps) {
  return <Spinner {...props} className={clsx('app-spinner', props.className)} />
}
