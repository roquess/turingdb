'use client'

import Image from 'next/image'
import Link from 'next/link'
import { usePathname } from 'next/navigation'
import clsx from 'clsx'
import AppTooltip from '@/components/base/AppTooltip'

interface LayoutSidebarItemCommonProps {
  href: string
  name: string
}

type LayoutSidebarItemProps =
  | (LayoutSidebarItemCommonProps & {
      icon: any
      img?: never
    })
  | (LayoutSidebarItemCommonProps & {
      img: string
      icon?: never
    })

export default function LayoutSidebarItem(props: LayoutSidebarItemProps) {
  const pathname = usePathname()
  const { href, name, icon, img } = props

  return (
    <AppTooltip label={name} interactionKind="hover-target">
      <Link
        href={href}
        className={clsx(
          [
            pathname === href
              ? 'sidebar-item-border sidebar-item-bg-gradient bg-grey-600 !text-content-primary'
              : 'bg-grey-800 !text-content-fourth hover:bg-grey-700',
          ],
          'flex h-10 w-10 items-center justify-center rounded-[4px] transition-colors'
        )}
      >
        {icon ? icon : <Image src={img || ''} width="40" height="40" alt="" />}
        <span className="sr-only">{name}</span>
      </Link>
    </AppTooltip>
  )
}
