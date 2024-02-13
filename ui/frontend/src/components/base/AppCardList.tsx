import { CardList, CardListProps } from '@blueprintjs/core'
import clsx from 'clsx'
import { useEffect, useRef, useState } from 'react'

interface AppCardListProps extends CardListProps {}

export default function AppCardList(props: AppCardListProps) {
  const listRef = useRef<HTMLDivElement>(null)
  const [isScrollbarVisible, setIsScrollbarVisible] = useState(false)
  const checkForScrollbar = () => {
    if (!listRef.current) return
    const { clientHeight, scrollHeight } = listRef.current
    setIsScrollbarVisible(scrollHeight > clientHeight)
  }
  useEffect(() => {
    checkForScrollbar()
  }, [])

  return (
    <CardList
      ref={listRef}
      {...props}
      className={clsx(
        'app-card-list custom-scrollbar',
        {
          'app-card-list--scrollbar-visible': isScrollbarVisible,
        },
        props.className
      )}
    >
      {props.children}
    </CardList>
  )
}
