import IconSearch from '@/assets/icons/search.svg'
import IconClose from '@/assets/icons/close-lg.svg'

import AppInput, { AppInputProps } from '@/components/base/AppInput'
import clsx from 'clsx'

export interface Props extends AppInputProps {
  onClear?: () => void
  clearAlwaysVisible?: boolean
  onIconClick?: () => void
}

export default function AppInputSearch(props: Props) {
  const { onClear, clearAlwaysVisible, onIconClick, ...rest } = props
  const maybeClear =
    clearAlwaysVisible || props.value?.length ? (
      <IconClose className="cursor-pointer" onClick={onClear} />
    ) : undefined

  return (
    <AppInput
      {...rest}
      leftElement={<IconSearch onClick={onIconClick} />}
      rightElement={maybeClear}
      className={clsx('app-input-search', props.className)}
    />
  )
}
