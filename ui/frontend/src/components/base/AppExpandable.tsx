import { Collapse, CollapseProps } from '@blueprintjs/core'
import { useState } from 'react'

interface Props extends CollapseProps {
  trigger: ({
    isExpanded,
    setIsExpanded,
  }: {
    isExpanded: boolean
    setIsExpanded: (isExpanded: boolean) => void
  }) => React.ReactNode
  hideTriggerOnExpand?: boolean
}

export default function AppExpandable(props: Props) {
  const { hideTriggerOnExpand, trigger, children, ...rest } = props
  const [isExpanded, setIsExpanded] = useState(false)
  const showTrigger = !hideTriggerOnExpand || !isExpanded

  return (
    <>
      {showTrigger && trigger({ isExpanded, setIsExpanded })}
      <Collapse isOpen={isExpanded} {...rest}>
        {children}
      </Collapse>
    </>
  )
}
